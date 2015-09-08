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

#include "commandQueue.h"

#include "commandFactory.h"
#include "../data/dataStore.h"

CommandQueue* CommandQueue::_instance = NULL;

CommandQueue::CommandQueue(QObject *parent) : QObject(parent)
{

}

void CommandQueue::enqueueCommand(AbstractCommand *command)
{
    _commands.enqueue(command);
    connect(command, &AbstractCommand::statusChanged, DataStore::getInstance(), &DataStore::on_CommandStatusChange);
}

AbstractCommand *CommandQueue::getCommand()
{
    return _commands.empty()?NULL:_commands.dequeue();
}

CommandQueue *CommandQueue::getInstance()
{
    if ( NULL == _instance ) {

        _instance = new CommandQueue();

    }

    return _instance;
}

void CommandQueue::on_scanDarkLibrary()
{
    AbstractCommand* command =
            CommandFactory::createScanDarkSourceCommand(DataStore::getInstance()->getDarkSources());

    enqueueCommand(command);
    emit createdCommand(command);

}
