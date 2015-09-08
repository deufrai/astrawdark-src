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
#include "commandQueue.h"
#include "../data/dataStore.h"

#include <QThread>
#include <iostream>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

CommandManager* CommandManager::_instance = NULL;

CommandManager::CommandManager(QObject *parent)
    : QObject(parent), _running(true)
{

}

CommandManager* CommandManager::getInstance() {

    if ( NULL == _instance ) {

        _instance = new CommandManager();
    }

    return _instance;
}

void CommandManager::start()
{
    CommandManager* manager = getInstance();
    manager->run();
}

void CommandManager::stop()
{
    getInstance()->setRunning(false);
}

void CommandManager::run()
{
#ifndef QT_NO_DEBUG
    qDebug() << "CommandManager is running";
#endif

    while(_running) {

        QThread::currentThread()->msleep(100);
        AbstractCommand* command = CommandQueue::getInstance()->getCommand();

        if ( command ) {

            command->execute();
            delete command;
        }
    }
}
