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

#ifndef LIGHTSCHECKCOMMAND_H
#define LIGHTSCHECKCOMMAND_H

#include <QObject>

#include "abstractCommand.h"

/**
 * @brief checks if all scanned lights are
 *
 *  - from the same camera
 *  - shot with the same ISO & exposure time
 */
class LightsCheckCommand : public AbstractCommand
{
    Q_OBJECT

public:
    /**
     * Constructor
     */
    LightsCheckCommand();

protected:
    /** processing payload */
    virtual void do_processing();

signals:

public slots:
};

#endif // LIGHTSCHECKCOMMAND_H
