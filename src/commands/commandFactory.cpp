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

#include "commandFactory.h"
#include "scanDarkSourceCommand.h"

#include "../data/dataStore.h"

CommandFactory::CommandFactory()
{

}

AbstractCommand *CommandFactory::createScanDarkSourceCommand(const QStringList& sources)
{
    ScanDarkSourceCommand* command = new ScanDarkSourceCommand(sources);
    command->connect(command, &ScanDarkSourceCommand::done, DataStore::getInstance(), &DataStore::on_newDarkScanResult);
    return command;
}

