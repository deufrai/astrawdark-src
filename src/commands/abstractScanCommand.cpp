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

#include "abstractScanCommand.h"
#include "../data/access/exifReader.h"

#include <QDirIterator>

AbstractScanCommand::AbstractScanCommand(const QStringList sources)
    : _sources(sources)
{

}

void AbstractScanCommand::getRawPathsInDirectory(const QString directory)
{
    /*
     * retrieve paths of all RAW files located in current dark source folder,
     * including subdirectories
     */
    _progressMessage = tr("Locating raw files...");
    emit statusChanged(this);

    QDirIterator it(directory,
                    QStringList() << "*.CR2" << "*.CRW",
                    QDir::NoDotAndDotDot | QDir::Files,
                    QDirIterator::Subdirectories);

    int found=0;
    while (it.hasNext()) {

        _imagePaths << it.next();
        _progressMessage = tr("%1 RAW file(s) found").arg(++found);
        emit statusChanged(this);
    }

    _progressMessage = tr("%1 RAW file(s) found").arg(found);
    emit statusChanged(this);
}

void AbstractScanCommand::retrieveExifMetadata()
{
    long fileCount = 0;

    /*
     * Retrieve EXIF metadata for each RAW file
     */
    foreach (QString path, _imagePaths) {

        ImageInfo imageInfo(path);

        if ( ! ExifReader::retrieveExifMetadata(imageInfo) ) {

            _badFilesPaths << imageInfo.getPath();

        }
        else if ( imageInfo.getExposure() < 1 ) {

            _shortFilesPaths << imageInfo.getPath();

        } else {

            _imageInfos << imageInfo;
        }

        _progressMessage = tr("Scanned file %1 / %2").arg(++fileCount).arg(_imagePaths.count());
        emit statusChanged(this);
    }
}

AbstractScanCommand::~AbstractScanCommand()
{

}

void AbstractScanCommand::checkForErrors()
{
    if ( ! _shortFilesPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Ignored %1 short exposure image(s)")
                .arg(_shortFilesPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _shortFilesPaths);
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

    }
}

