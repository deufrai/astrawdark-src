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
    ImageInfo(const QString path);

protected:
    QString _path;
    QString _make;
    QString _model;
    QString _exposure;
    QString _iso;
    QString _date;
    int     _temperature;

public:
    const QString getPath() const { return _path; }
    const QString getMake() const { return _make;}
    const QString getModel() const { return _model;}
    const QString getExposure() const { return _exposure;}
    const QString getIso() const { return _iso;}
    const QString getDate() const { return _date;}
    int getTemperature() const { return _temperature;}

    void setPath (const QString path) { _path = path;}
    void setMake (const QString make) { _make = make;}
    void setModel (const QString model) { _model = model;}
    void setExposure (const QString exposure) { _exposure = exposure;}
    void setIso (const QString iso) { _iso = iso;}
    void setDate (const QString date) { _date = date;}
    void setTemperature (const int temperature) { _temperature = temperature;}

};

#endif // IMAGEINFO_H
