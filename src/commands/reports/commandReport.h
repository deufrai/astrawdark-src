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

#ifndef COMMANDREPORT_H
#define COMMANDREPORT_H

#include <QStringList>

/**
 * @brief A set of messages & lists to store a command's processing report
 */
class CommandReport
{
public:
    /**
     * Constructor
     */
    CommandReport();

    /**
     * Add a report section
     * @param header section header
     * @param content section content
     */
    void addSection(QString header, QStringList content);

    /**
     * Get all report headers
     * @return a list of headers
     */
    QStringList getHeaders() const {return _headers;}

    /**
     * Get all report contents
     * @return a list of contents
     */
    QList<QStringList> getContents() const {return _contents;}

private:
    /** Report section headers */
    QStringList             _headers;

    /** Report section content */
    QList<QStringList>      _contents;



};

#endif // COMMANDREPORT_H
