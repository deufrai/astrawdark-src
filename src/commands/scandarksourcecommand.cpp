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
#include <QDirIterator>

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

        // retrieve all RAW files located in <path> directory and subtree
        QDirIterator it(QString(_path.c_str()),
                        QStringList() << "*.CR2" << "*.CRW",
                        QDir::NoDotAndDotDot | QDir::Files,
                        QDirIterator::Subdirectories);

        QList<ImageInfo*> imageInfos;

        // retrieve all needed exif metadata for each RAW file
        while (it.hasNext()) {

            ImageInfo* imageInfo = new ImageInfo(it.next().toStdString());

            ExifReader::retrieveExifMetadata(*imageInfo);

            imageInfos << imageInfo;
        }

        // push images infos to data store
        DataStore::getInstance()->on_newDarkScanResult(imageInfos);
    }
}

