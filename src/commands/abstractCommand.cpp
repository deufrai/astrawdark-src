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
#include "signalDispatcher.h"

int AbstractCommand::SERIAL = 0;

AbstractCommand::AbstractCommand()
    : _status(SCHEDULED),
      _elapsed(0),
      _serial(SERIAL++),
      _error(false),
      _warning(false) {

    connect(this,
            &AbstractCommand::statusChanged,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_CommandStatusChange);
}

AbstractCommand::~AbstractCommand()
{

}

void AbstractCommand::execute()
{
    /*
     * this is a function template, all it does
     * is calling redefined functions from subclasses
     */

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

const QString AbstractCommand::getStatusString() const
{
    QString sRet;

    switch ( _status ) {

    case AbstractCommand::SCHEDULED:
        sRet = tr("Scheduled");
        break;

    case AbstractCommand::RUNNING:
        sRet = tr("Running");
        break;
    case AbstractCommand::COMPLETE:
        sRet = tr("Finished (%1 ms)").arg(getElapsed());
        break;

    default:
        sRet = tr("Undefined");
        break;
    }

    return sRet;
}

