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

#include <QString>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

QString ExifReader::NOT_AVAILABLE = "N/A";

ExifReader::ExifReader()
{

}

void ExifReader::retrieveExifMetadata(ImageInfo &imageInfo)
{
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
        imageInfo.setIso(getValue(exifData, "Exif.Photo.ISOSpeedRatings"));
        imageInfo.setDate(getValue(exifData, "Exif.Photo.DateTimeDigitized"));

        QString temp = getValue(exifData, "Exif.CanonSi.0x000c");

        if ( temp == NOT_AVAILABLE ) {

            imageInfo.setTemperature(-1);

        } else {

            imageInfo.setTemperature(temp.toInt() -128);
        }

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

        return NOT_AVAILABLE;

    } else {

        return QString(pos->getValue()->toString().c_str());
    }
}

QString ExifReader::formatExposure(QString unformatted)
{
    if ( ! unformatted.isEmpty() ) {

        // we get rid of "/1" suffixes
        if ( unformatted.endsWith("/1") ) return unformatted.remove("/1");

        // we simplify "x/10" values but leave "1/10" special case alone
        if ( unformatted.endsWith("/10") && unformatted != "1/10" ) {

            int numerator = unformatted.left(unformatted.indexOf('/')).toInt();
            double value = numerator / 10.0;
            return QString::number(value);
        }
    }

    return unformatted;
}

