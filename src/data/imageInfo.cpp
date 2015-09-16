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

#include "imageInfo.h"

QString ImageInfo::NOT_AVAILABLE = "N/A";

ImageInfo::ImageInfo(const QString path)
    : _path(path),
      _make(NOT_AVAILABLE),
      _model(NOT_AVAILABLE),
      _cameraSerial(NOT_AVAILABLE),
      _exposure(UNDEFINED),
      _iso(UNDEFINED),
      _date(NOT_AVAILABLE),
      _temperature(UNDEFINED)
{}

ImageInfo::ImageInfo()
    : _path(NOT_AVAILABLE),
      _make(NOT_AVAILABLE),
      _model(NOT_AVAILABLE),
      _cameraSerial(NOT_AVAILABLE),
      _exposure(UNDEFINED),
      _iso(UNDEFINED),
      _date(NOT_AVAILABLE),
      _temperature(UNDEFINED)
{

}

ImageInfo::ImageInfo(const ImageInfo &info)
    : _path(QString(info.getPath())),
      _make(QString(info.getMake())),
      _model(QString(info.getModel())),
      _cameraSerial(QString(info.getCameraSerial())),
      _exposure(info.getExposure()),
      _iso(info.getIso()),
      _date(QString(info.getDate())),
      _temperature(info.getTemperature())
{

}
