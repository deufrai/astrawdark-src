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

#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include "abstractCommand.h"
#include <string>

/**
 * @brief Command factory in charge of instanciating all command types in a static
 * function for each type.
 */
class CommandFactory
{
private:
    /** Constructor */
    CommandFactory();

public:
    /**
     * Create a ScanDarkSourceCommand.
     * @param sources a list of dark frame source folder paths
     * @return a pointer to the newly created command
     */
    static AbstractCommand* createScanDarkSourceCommand(const QStringList &sources);

};

#endif // COMMANDFACTORY_H
