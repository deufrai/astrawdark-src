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

#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H

#include <QObject>
#include <QQueue>
#include "abstractCommand.h"

/**
 * @brief Queue that holds commands that need to be executed.
 */
class CommandQueue : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param parent Qt parent
     */
    explicit CommandQueue(QObject *parent = 0);

    /** Get the next scheduled command */
    AbstractCommand* getCommand();

    /** Get the command that has serial number n */
    AbstractCommand* getCommand(int serial);

    /**
     * Put a command at the back of the queue
     * @param command command to be queued
     */
    void enqueueCommand(AbstractCommand* command);

    /**
     * Used to check if the queue contains a scheduled command
     * @return true if queue has scheduled commands
     */
    bool hasCommands() const;

private:
    /** the command queue */
    QQueue<AbstractCommand*> _commands;

    /**
     * Get the next pending command
     * @return a pointer to next pending command
     */
    AbstractCommand* getScheduledCommand() const;

signals:
    /**
     * Advertise that a command has been queued
     */
    void commandEnqueued(AbstractCommand*);

};

#endif // COMMANDQUEUE_H
