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

#include "exifreader.h"
#include "../imageinfo.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ExifReader::ExifReader()
{

}

void ExifReader::getMetaData(ImageInfo &imageInfo)
{
    Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(imageInfo.getPath());
    image->readMetadata();

    Exiv2::ExifData &exifData = image->exifData();

    if (exifData.empty()) {

#ifndef QT_NO_DEBUG
        qWarning() << imageInfo.getPath().c_str() << ": No Exif data found in the file";
#endif


    } else {

        imageInfo.setMake(getValue(exifData, "Exif.Image.Make"));
        imageInfo.setModel(getValue(exifData, "Exif.Image.Model"));
        imageInfo.setExposure(getValue(exifData, "Exif.Photo.ExposureTime"));
        imageInfo.setIso(getValue(exifData, "Exif.Photo.ISOSpeedRatings"));
        imageInfo.setDate(getValue(exifData, "Exif.Photo.DateTimeDigitized"));

        std::string temp = getValue(exifData, "Exif.CanonSi.0x000c");

        if ( temp == "N/A" ) {

            imageInfo.setTemperature(-1);

        } else {

           imageInfo.setTemperature(atoi(temp.c_str()) -128);
        }

    }
}

std::string ExifReader::getValue(const Exiv2::ExifData &data, const std::string tag)
{
    Exiv2::ExifKey exifKey(tag);
    Exiv2::ExifData::const_iterator pos = data.findKey(exifKey);

    if (pos == data.end()) {

        return "N/A";

    } else {

        return pos->getValue()->toString();
    }
}

