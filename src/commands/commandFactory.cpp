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
#include "scanLightsCommand.h"
#include "lightsCheckCommand.h"
#include "matchDarksCommand.h"

#include "data/dataStore.h"

CommandFactory::CommandFactory()
{

}

AbstractCommand *CommandFactory::createScanDarkSourceCommand(const QStringList &sources)
{
    return new ScanDarkSourceCommand(sources);
}

AbstractCommand *CommandFactory::createScanLightsCommand(const QString dir)
{
    return new ScanLightsCommand(dir);
}

AbstractCommand *CommandFactory::createLightsCheckCommand()
{
    return new LightsCheckCommand();
}

AbstractCommand* CommandFactory::createMatchDarksCommand() {

	return new MatchDarksCommand();
}
