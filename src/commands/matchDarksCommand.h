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

#ifndef SRC_COMMANDS_MATCHDARKSCOMMAND_H_
#define SRC_COMMANDS_MATCHDARKSCOMMAND_H_

#include "abstractCommand.h"

/**
 * @brief Command that creates a list of darks
 * from a list of lights.
 *
 */
class MatchDarksCommand : public AbstractCommand {

	Q_OBJECT
public:
	MatchDarksCommand();
	virtual ~MatchDarksCommand();

protected:
    /** processing payload */
    virtual void do_processing();
};

#endif /* SRC_COMMANDS_MATCHDARKSCOMMAND_H_ */
