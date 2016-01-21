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
#include "reports/commandReport.h"


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

    /**
     * Destructor
     */
    ~CommandManager();

private:
    /** the command queue */
    CommandQueue*   _queue;
    /** the command executor */
    CommandExecutor _executor;

public:
    /**
     * Get a command by its serial number
     *
     * @param serial the command's serial number
     * @return the command
     */
    const AbstractCommand* getCommand(int serial);

public slots:
    /** Slot triggered by dark library scan requests */
    void on_createDarkScanCommand();

    /** Slot triggered by lights folder scan requests */
    void on_createLightsScanCommand();

    /** Slot triggered by lights check requests */
    void on_createLightsCheckCommand();

    /** Slot triggered by darks matching requests */
    void on_createMatchDarksCommand();



};

#endif // COMMANDMANAGER_H
