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

#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <QtConcurrent>

#include "abstractCommand.h"
#include "commandQueue.h"

/**
 * @brief The Command Executor is in charge of executing all application commands.
 *
 * It justs keeps polling the command queue and executes every available command
 * in an isolated thread.
 */
class CommandExecutor
{
public:
    /**
     * Constructor
     * @param queue Pointer to the unique command queue
     */
    explicit CommandExecutor(CommandQueue* queue);

    /**
     * Desctructor
     */
    ~CommandExecutor();

    /**
     * Start polling the command queue
     */
    void start();

    /**
     * Stop polling the command queue
     */
    void stop();

private:
    /**
     * poll command queue in an infinite loop
     * @param queue Pointer to the unique command queue
     */
    static void run(CommandQueue* queue);

    /** Pointer to the command queue */
    CommandQueue* _queue;

    /** flag used to stop polling the command queue */
    static bool   _keepRunning;

};

#endif // COMMANDEXECUTOR_H
