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

#include "abstractCommand.h"
#ifndef QT_NO_DEBUG
#include <QDebug>
#include <QElapsedTimer>
#endif

AbstractCommand::AbstractCommand()
{

}

AbstractCommand::~AbstractCommand()
{

}

void AbstractCommand::execute()
{
#ifndef QT_NO_DEBUG
    QElapsedTimer t;
    t.start();
    qDebug () << "Command:" << _description << "started";
#endif

    setup();
    do_processing();
    cleanup();

#ifndef QT_NO_DEBUG
    qDebug () << "Command:" << _description << "completed in" << t.elapsed() << "ms";
#endif
}

