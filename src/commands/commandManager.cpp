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

#include "commandManager.h"
#include "commandFactory.h"
#include "../data/dataStore.h"
#include "../globals.h"
#include "signalDispatcher.h"

#include <QThread>
#include <QSettings>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

CommandManager::CommandManager(QObject *parent)
    : QObject(parent),
      _queue(new CommandQueue()),
      _executor(_queue)
{
    QObject::connect(SignalDispatcher::getInstance(),
                     &SignalDispatcher::createDarkScanCommand,
                     this,
                     &CommandManager::on_createDarkScanCommand);

    _executor.start();

    // launch a dark scan on startup, if user wants it
    if ( QSettings().value(Globals::SETTINGKEY_SCANDARKS_ON_STARTUP, false).toBool() ) {

        on_createDarkScanCommand();
    }
}

CommandManager::~CommandManager()
{
    _executor.stop();
}

void CommandManager::on_createDarkScanCommand()
{
    AbstractCommand* command =
            CommandFactory::createScanDarkSourceCommand(DataStore::getInstance()->getDarkSources());

    _queue->enqueueCommand(command);
}
