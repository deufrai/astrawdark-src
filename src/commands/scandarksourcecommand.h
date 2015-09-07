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

#ifndef SCANDARKSOURCE_H
#define SCANDARKSOURCE_H

#include "abstractcommand.h"

#include <string>


class ScanDarkSourceCommand : public AbstractCommand
{
public:
    ScanDarkSourceCommand(const std::string path);
    virtual ~ScanDarkSourceCommand() {}

protected:
    virtual void do_processing();

private:
    const std::string _path;

};

#endif // SCANDARKSOURCE_H
