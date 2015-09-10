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

#include "../data/dataStore.h"

CommandQueue::CommandQueue(QObject *parent) : QObject(parent)
{
    connect(this,
            &CommandQueue::commandEnqueued,
            DataStore::getInstance(),
            &DataStore::on_CommandCreated);
}

void CommandQueue::enqueueCommand(AbstractCommand *command)
{
    connect(command, &AbstractCommand::statusChanged, DataStore::getInstance(), &DataStore::on_CommandStatusChange);
    _commands.enqueue(command);
    emit commandEnqueued(command);
}

AbstractCommand* CommandQueue::getCommand()
{
    return _commands.dequeue();
}

