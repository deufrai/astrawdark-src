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

#include "exifReader.h"
#include "../imageInfo.h"

#include <QStringList>
#include <QString>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ExifReader::ExifReader()
{

}

bool ExifReader::retrieveExifMetadata(ImageInfo &imageInfo)
{
    try {

        // Read EXIF bulk data from image
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(imageInfo.getPath().toStdString());
        image->readMetadata();
        Exiv2::ExifData &exifData = image->exifData();

        if (exifData.empty()) {

#ifndef QT_NO_DEBUG
            qWarning() << imageInfo.getPath() << ": No Exif data found in the file";
#endif


        } else {

            // extract relevant EXIF values
            imageInfo.setMake(getValue(exifData, "Exif.Image.Make"));
            imageInfo.setModel(getValue(exifData, "Exif.Image.Model"));

            imageInfo.setExposure(formatExposure(getValue(exifData, "Exif.Photo.ExposureTime")));

            if ( imageInfo.getExposure() == ImageInfo::UNDEFINED ) {

                return false;
            }


            QString tempIso = getValue(exifData, "Exif.Photo.ISOSpeedRatings");

            if ( tempIso == ImageInfo::NOT_AVAILABLE ) {

                imageInfo.setIso(ImageInfo::UNDEFINED);
                return false;

            } else {

                imageInfo.setIso(tempIso.toInt());
            }

            imageInfo.setDate(getValue(exifData, "Exif.Photo.DateTimeDigitized"));

            QString tempTemp = getValue(exifData, "Exif.CanonSi.0x000c");

            if ( tempTemp == ImageInfo::NOT_AVAILABLE ) {

                imageInfo.setTemperature(ImageInfo::UNDEFINED);
                return false;

            } else {

                imageInfo.setTemperature(tempTemp.toInt() -128);
            }

            QString serial = getValue(exifData, "Exif.Photo.BodySerialNumber");

            if ( ImageInfo::NOT_AVAILABLE == serial ) {

                serial = getValue(exifData, "Exif.Canon.SerialNumber");
            }

            imageInfo.setCameraSerial(serial);

        }

        return true;

    } catch ( Exiv2::Error e ) {

#ifndef QT_NO_DEBUG
        qWarning() << e.what();
#endif

        return false;

    }
}

QString ExifReader::getValue(const Exiv2::ExifData &data, const QString tag)
{
    /*
     * retrieve value for specific EXIF tag.
     * return "N/A" if tag is missing
     */
    Exiv2::ExifKey exifKey(tag.toStdString());
    Exiv2::ExifData::const_iterator pos = data.findKey(exifKey);

    if (pos == data.end()) {

        return ImageInfo::NOT_AVAILABLE;

    } else {

        return QString(pos->getValue()->toString().c_str());
    }
}

int ExifReader::formatExposure(QString expoString)
{
    if ( expoString.isEmpty() || ImageInfo::NOT_AVAILABLE == expoString ) {

        return ImageInfo::UNDEFINED;
    }

    double dRet = 0.0;

    if ( expoString.contains('/') ) {

        int num   = QString(expoString.split('/').at(0)).toInt();
        int denom = QString(expoString.split('/').at(1)).toInt();

        dRet = num / static_cast<double> (denom);

    } else {

        dRet = expoString.toDouble();
    }



    return qRound(dRet);

}

