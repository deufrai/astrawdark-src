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

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>

#include "commandExecutor.h"
#include "commandQueue.h"


/**
 * @brief Manages commands, from creation to queueing and execution.
 *
 * It is mostly in charge of recieving command creation requests from the GUI
 * It enqueues those commands before they are picked up by the CommandExecutor.
 */
class CommandManager : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent Qt parent
     */
    explicit CommandManager(QObject *parent = 0);
    ~CommandManager();

private:
    /** the command queue */
    CommandQueue*   _queue;
    /** the command executor */
    CommandExecutor _executor;

signals:

public slots:
    /** Slot triggered by dark library scan requests */
    void on_createDarkScanCommand();

};

#endif // COMMANDMANAGER_H
