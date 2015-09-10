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

#ifndef EXIFREADER_H
#define EXIFREADER_H

#include <exiv2/exiv2.hpp>
#include <QString>

class ImageInfo;

/**
 * @brief Utility class for all Exif metadata accesses.
 */
class ExifReader
{
private:
    /**
     * Constructor.
     *
     * All functions are static, so constructor is private
     */
    ExifReader();

public:
    /**
     * Retrieve EXIF metadata for an ImageInfo and populates it in-place.
     *
     * @param imageInfo The ImageInfo to populate
     */
    static void retrieveExifMetadata(ImageInfo& imageInfo);

private:
    /** Constant used to replace non-existant values */
    static QString NOT_AVAILABLE;

    /**
     * Extract EXIF value from EXIF metadata for a specific EXIF tag
     *
     * @param data the EXIF metadata
     * @param tag  te EXIF tag
     * @return the corresponding EXIF value
     */
    static QString getValue(const Exiv2::ExifData &data, const QString tag);

    /**
     * Format exposure time for human
     *
     * @param unformatted exposure time to be formatted
     * @return formatted exposure time
     */
    static QString formatExposure(QString unformatted);

};

#endif // EXIFREADER_H
