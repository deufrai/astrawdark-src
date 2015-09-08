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

quint64 AbstractCommand::SERIAL = 0;

AbstractCommand::AbstractCommand()
{
    _status = PENDING;
    _elapsed = 0;
    _serial = SERIAL++;
}

AbstractCommand::~AbstractCommand()
{

}

void AbstractCommand::execute()
{
    _timer.start();
    _status = RUNNING;
    emit statusChanged(this);

    setup();
    do_processing();
    cleanup();

    _elapsed = _timer.elapsed();
    _status = COMPLETE;
    emit statusChanged(this);
}

