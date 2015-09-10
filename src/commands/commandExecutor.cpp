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

#include "commandExecutor.h"

bool CommandExecutor::_keepRunning = true;

CommandExecutor::CommandExecutor(CommandQueue *queue)
    : _queue(queue)
{

}

CommandExecutor::~CommandExecutor()
{

}

void CommandExecutor::start()
{
    /*
     * execute run() function in an isolated thread
     */

    QtConcurrent::run(&CommandExecutor::run, _queue);
}

void CommandExecutor::stop()
{
    _keepRunning = false;
}

void CommandExecutor::run(CommandQueue* queue)
{
#ifndef QT_NO_DEBUG
    qDebug() << "CommandExecutor starting...";
#endif

    /*
     * check infinitly for commands to execute
     */
    while(_keepRunning) {

        QThread::currentThread()->msleep(10);

        if ( queue->hasCommands() ) {

            queue->getCommand()->execute();
        }
    }

#ifndef QT_NO_DEBUG
    qDebug() << "CommandExecutor stopped";
#endif
}
