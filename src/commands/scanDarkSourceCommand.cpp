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
#include "../data/dataStore.h"

#include <QStringList>
#include <QFileInfoList>
#include <QDirIterator>
#include <QObject>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ScanDarkSourceCommand::ScanDarkSourceCommand(const QStringList &sources)
    : AbstractCommand::AbstractCommand(), _sources(sources)
{
    _description = QString(tr("Scaning dark library"));
}

void ScanDarkSourceCommand::do_processing()
{
    if ( ! _sources.empty() ) {

        QList<ImageInfo> imageInfos;

        foreach (QString path, _sources) {

            // retrieve all RAW files located in <path> directory, with subtree
            QDirIterator it(path,
                            QStringList() << "*.CR2" << "*.CRW",
                            QDir::NoDotAndDotDot | QDir::Files,
                            QDirIterator::Subdirectories);

            while (it.hasNext()) {

                ImageInfo imageInfo(it.next());
                ExifReader::retrieveExifMetadata(imageInfo);
                imageInfos << imageInfo;
            }
        }

        // register results in datastore
        emit done(imageInfos);
    }
}

