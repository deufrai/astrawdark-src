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
 * @brief Manages commands from their creation to their execution in
 * an isolated thread.
 */
class CommandManager : public QObject
{
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = 0);
    ~CommandManager();

private:
    CommandQueue*   _queue;
    CommandExecutor _executor;

signals:

public slots:
    void on_scanDarkLibrary();

};

#endif // COMMANDMANAGER_H
