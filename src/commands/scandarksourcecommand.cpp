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

#include <QStringList>
#include <QFileInfoList>
#include <QDir>
#include <exiv2/exiv2.hpp>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ScanDarkSourceCommand::ScanDarkSourceCommand(const char *path)
    : _path(path)
{


}

void ScanDarkSourceCommand::do_processing()
{
    if ( 0 != _path ) {

        QStringList extensions;
        extensions << "*.CR2" << "*.CRW";
        QFileInfoList fileInfos = QDir(_path).entryInfoList(extensions,
                                                            QDir::NoDotAndDotDot | QDir::Files,
                                                            QDir::Name);

#ifndef QT_NO_DEBUG
        qDebug() << "Found" << fileInfos.size() << "files";
#endif

        QList<ImageInfo*> imageInfos;

        foreach (QFileInfo fileInfo, fileInfos) {

            ImageInfo* imageInfo = new ImageInfo(fileInfo.filePath().toStdString());

            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(fileInfo.filePath().toStdString());
            image->readMetadata();

            Exiv2::ExifData &exifData = image->exifData();

            if (exifData.empty()) {

                qWarning() << fileInfo.filePath() << ": No Exif data found in the file";

            } else {

                imageInfo->setMake(exifData["Exif.Image.Make"].value().toString());
                imageInfo->setModel(exifData["Exif.Image.Model"].value().toString());
                imageInfo->setExposure(exifData["Exif.Photo.ExposureTime"].value().toString());
                imageInfo->setIso(exifData["Exif.Photo.ISOSpeedRatings"].value().toString());
                imageInfo->setDate(exifData["Exif.Photo.DateTimeDigitized"].value().toString());
                imageInfo->setTemperature(exifData["Exif.CanonSi.0x000c"].value().toString());
            }

            imageInfos << imageInfo;
        }

        DataStore::getInstance()->registerDarks(imageInfos);
    }
}

