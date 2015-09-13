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

void ScanDarkSourceCommand::checkForErrors()
{
    if ( ! _shortFilesPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - We found %1 short exposure image(s) in your dark library :")
                .arg(_shortFilesPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _shortFilesPaths);
    }

    /*
     * If dark sources were missing, update error message
     */
    if ( ! _missingDirsPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Some of your dark sources folders are missing :");

        _reportMessages << msg;

        _commandReport.addSection(msg, _missingDirsPaths);
    }

    /*
     * If we saw bad files, update error message
     */
    if ( ! _badFilesPaths.isEmpty() ) {

        _error = true;

        QString msg = tr("ERROR - We found %1 invalid RAW file(s) in your dark library :")
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

void ScanDarkSourceCommand::do_processing()
{
    emit started();

    if ( ! _sources.isEmpty() ) {

        QList<QString>      imagePaths;
        QList<ImageInfo>    imageInfos;

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
            _progressMessage = tr("Locating raw files...");
            emit statusChanged(this);

            QDirIterator it(path,
                            QStringList() << "*.CR2" << "*.CRW",
                            QDir::NoDotAndDotDot | QDir::Files,
                            QDirIterator::Subdirectories);

            _progressMessage = tr("Found 0 raw file");
            emit statusChanged(this);

            while (it.hasNext()) {

                imagePaths << it.next();
            }
        }

        long fileCount = 0;

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

            _progressMessage = tr("Scanned file %1 / %2").arg(++fileCount).arg(imagePaths.count());
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

