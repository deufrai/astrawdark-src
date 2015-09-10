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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <QString>

/**
 * @brief Application-wide constants.
 */
class Globals
{
private:
    Globals();

public:
    /** application version string */
    static const QString VERSION_STRING;
    /** application name */
    static const QString APPLICATION_NAME;

    /** Key used to store dark sources */
    static const QString SETTINGKEY_DARK_SOURCES;
    /** Key for window geometry */
    static const QString SETTINGKEY_WINDOW_GEOMETRY;
    /** Key for 'remember window size & position' feature */
    static const QString SETTINGKEY_WINDOW_GEOMETRY_REMEMBER;

};

#endif // GLOBALS_H
