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

#include <string>

/**
 * @brief Image information.
 */
class ImageInfo
{
public:
    ImageInfo(const std::string path);

protected:
    std::string _path;
    std::string _make;
    std::string _model;
    std::string _exposure;
    std::string _iso;
    std::string _date;
    int _temperature;

public:
    const std::string getPath() const { return _path; }
    const std::string getMake() const { return _make;}
    const std::string getModel() const { return _model;}
    const std::string getExposure() const { return _exposure;}
    const std::string getIso() const { return _iso;}
    const std::string getDate() const { return _date;}
    int getTemperature() const { return _temperature;}

    void setPath (const std::string path) { _path = path;}
    void setMake (const std::string make) { _make = make;}
    void setModel (const std::string model) { _model = model;}
    void setExposure (const std::string exposure) { _exposure = exposure;}
    void setIso (const std::string iso) { _iso = iso;}
    void setDate (const std::string date) { _date = date;}
    void setTemperature (const int temperature) { _temperature = temperature;}

};

#endif // IMAGEINFO_H
