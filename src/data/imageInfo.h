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

#ifndef IMAGEINFO_H
#define IMAGEINFO_H

#include <QString>

/**
 * @brief Image information.
 */
class ImageInfo
{
public:
    /**
     * Constructor
     * @param path path of the image we store info for
     */
    ImageInfo(const QString path);

protected:
    /** Image path */
    QString _path;
    /** Camera manufacturer */
    QString _make;
    /** Camera model */
    QString _model;
    /** Exposure time */
    QString _exposure;
    /** Sensor ISO */
    QString _iso;
    /** Shoot date & time */
    QString _date;
    /** Sensor temperature */
    int     _temperature;

public:
    /**
     * Get image path
     * @return image path
     */
    const QString getPath() const { return _path; }

    /**
     * Get camera manufacturer
     * @return camera manufacturer
     */
    const QString getMake() const { return _make;}

    /**
     * Get camera model
     * @return camera model
     */
    const QString getModel() const { return _model;}

    /**
     * Get exposure time
     * @return exposure time
     */
    const QString getExposure() const { return _exposure;}

    /**
     * Get Sensor ISO
     * @return Sensor ISO
     */
    const QString getIso() const { return _iso;}

    /**
     * Get Shoot date & time
     * @return Shoot date & time
     */
    const QString getDate() const { return _date;}

    /**
     * Get Sensor temperature
     * @return Sensor temperature
     */
    int getTemperature() const { return _temperature;}

    /**
     * Set image path
     * @param path image path
     */
    void setPath (const QString path) { _path = path;}

    /**
     * Set camera manufacturer
     * @param make camera manufacturer
     */
    void setMake (const QString make) { _make = make;}

    /**
     * Set camera model
     * @param model camera model
     */
    void setModel (const QString model) { _model = model;}

    /**
     * Set exposure time
     * @param exposure exposure time
     */
    void setExposure (const QString exposure) { _exposure = exposure;}

    /**
     * Set Sensor ISO
     * @param iso Sensor ISO
     */
    void setIso (const QString iso) { _iso = iso;}

    /**
     * Set Shoot date & time
     * @param date Shoot date & time
     */
    void setDate (const QString date) { _date = date;}

    /**
     * Set Sensor temperature
     * @param temperature Sensor temperature
     */
    void setTemperature (const int temperature) { _temperature = temperature;}

};

#endif // IMAGEINFO_H
