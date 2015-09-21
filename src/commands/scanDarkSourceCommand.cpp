/*
 *  AstRawDark
 *
 *  Copyright (C) 2015 - Frédéric CORNU
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "scanDarkSourceCommand.h"
#include "../data/access/exifReader.h"
#include "signalDispatcher.h"

#include <QStringList>
#include <QFileInfoList>
#include <QDirIterator>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ScanDarkSourceCommand::ScanDarkSourceCommand(const QStringList &sources)
    : AbstractCommand::AbstractCommand(),
      _sources(sources)
{
    _description = QString(tr("Dark library scan"));

    connect(this,
            &ScanDarkSourceCommand::started,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_darkScanStarted);

    connect(this,
            &ScanDarkSourceCommand::done,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_darkScanDone);
}

void ScanDarkSourceCommand::do_processing()
{
    emit started();

    if ( ! _sources.isEmpty() ) {

        QList<QString>      imagePaths;
        QList<ImageInfo>    imageInfos;

        _progressMessage = tr("Looking for RAW files...");
        emit statusChanged(this);

        int found=0;

        foreach (QString path, _sources) {

            /*
             * If current dark source folder is missing,
             * we skip it and store its path for error display
             */
            if ( ! QDir(path).exists() ) {

                _missingDirsPaths << path;
                continue;
            }

            /*
             * retrieve paths of all RAW files located in current dark source folder,
             * including subdirectories
             */
            QDirIterator it(path,
                            QStringList() << "*.CR2" << "*.CRW",
                            QDir::NoDotAndDotDot | QDir::Files,
                            QDirIterator::Subdirectories);

            while (it.hasNext()) {

                imagePaths << it.next();
                _progressMessage = tr("%1 RAW file(s) found").arg(++found);
                emit statusChanged(this);

            }
        }

        _progressMessage = tr("%1 RAW file(s) found").arg(found);
        emit statusChanged(this);

        int scanned = 0;

        /*
         * Retrieve EXIF metadata for each RAW file
         */
        foreach (QString filePath, imagePaths) {

            ImageInfo imageInfo(filePath);

            if ( ! ExifReader::retrieveExifMetadata(imageInfo) ) {

                _badFilesPaths << imageInfo.getPath();

            }
            else if ( imageInfo.getExposure() < 1 ) {

                _shortFilesPaths << imageInfo.getPath();

            } else {

                imageInfos << imageInfo;
            }

            _progressMessage = tr("Scanned file %1 / %2").arg(++scanned).arg(imagePaths.count());
            emit statusChanged(this);
        }

        /*
         * If we saw short frames, update warnings & errors
         */
        checkForErrors();

        // tell the world we're done
        emit done(imageInfos);
    }
}

void ScanDarkSourceCommand::checkForErrors()
{
    if ( ! _shortFilesPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Ignored %1 short exposure image(s)")
                .arg(_shortFilesPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _shortFilesPaths);
    }

    /*
     * If dark sources were missing, update error message
     */
    if ( ! _missingDirsPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Ignored %1 missing dark source folder")
                .arg(_missingDirsPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _missingDirsPaths);
    }

    /*
     * If we saw bad files, update error message
     */
    if ( ! _badFilesPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Ignored %1 invalid RAW file(s)")
                .arg(_badFilesPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _badFilesPaths);
    }

    if ( ! _warning && ! _error ) {

        _reportMessages << tr("OK");
        _commandReport.addSection(tr("Completed successfully"),QStringList());

    } else {

        _reportMessages.insert(0, tr("Some items need your attention :"));
    }
}
