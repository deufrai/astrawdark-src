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

#include "scandarksourcecommand.h"
#include "../data/datastore.h"
#include "../data/imageinfo.h"
#include "../data/access/exifreader.h"

#include <QStringList>
#include <QFileInfoList>
#include <QDir>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ScanDarkSourceCommand::ScanDarkSourceCommand(const std::string path)
    : _path(path)
{


}

void ScanDarkSourceCommand::do_processing()
{
    if ( ! _path.empty() ) {

        // retrieve all RAW files located in <path> directory
        QStringList extensions;
        extensions << "*.CR2" << "*.CRW";
        QFileInfoList fileInfos = QDir(QString(_path.c_str())).entryInfoList(extensions,
                                                                             QDir::NoDotAndDotDot | QDir::Files,
                                                                             QDir::Name);

#ifndef QT_NO_DEBUG
        qDebug() << "Found" << fileInfos.size() << "files";
#endif

        QList<ImageInfo*> imageInfos;

        // retrieve all needed exif metadata for each RAW file
        foreach (QFileInfo fileInfo, fileInfos) {

            ImageInfo* imageInfo = new ImageInfo(fileInfo.filePath().toStdString());

            ExifReader::retrieveExifMetadata(*imageInfo);

            imageInfos << imageInfo;
        }

        // push images infos to data store
        DataStore::getInstance()->on_newDarkScanResult(imageInfos);
    }
}

