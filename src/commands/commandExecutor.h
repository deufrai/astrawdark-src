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

#include <QObject>
#include <QtConcurrent>

#include "abstractCommand.h"
#include "commandQueue.h"

class CommandExecutor : public QObject
{
    Q_OBJECT
public:
    explicit CommandExecutor(CommandQueue* queue, QObject *parent = 0);
    ~CommandExecutor();
    void start();
    void stop();

private:
    static bool _running;

    static void run(CommandQueue* queue);

    CommandQueue* _queue;

signals:

public slots:
};

#endif // COMMANDEXECUTOR_H
