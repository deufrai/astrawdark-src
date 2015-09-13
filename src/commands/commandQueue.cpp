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
#include "signalDispatcher.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

CommandQueue::CommandQueue(QObject *parent) : QObject(parent)
{
    connect(this,
            &CommandQueue::commandEnqueued,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_commandCreated);
}

void CommandQueue::enqueueCommand(AbstractCommand *command)
{
    _commands.enqueue(command);
    emit commandEnqueued(command);
}

bool CommandQueue::hasCommands() const
{
    return NULL != getScheduledCommand();
}

AbstractCommand *CommandQueue::getScheduledCommand() const
{
    foreach (AbstractCommand* command, _commands) {

        if ( AbstractCommand::SCHEDULED == command->getStatus() ) {

            return command;
        }
    }
    return NULL;
}

AbstractCommand* CommandQueue::getCommand()
{
    return getScheduledCommand();
}

AbstractCommand *CommandQueue::getCommand(int serial)
{
    foreach (AbstractCommand* command, _commands) {

#ifndef QT_NO_DEBUG
        qDebug() << QString("Inspecting command %1, seeking for serial# %2").arg(command->getSerial()).arg(serial);
#endif

        if ( serial == command->getSerial() ) {

            return command;
        }
    }
    return NULL;
}



