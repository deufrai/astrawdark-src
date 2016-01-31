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

#ifndef SRC_COMMANDS_COMPUTEBESTMATCHCOMMAND_H_
#define SRC_COMMANDS_COMPUTEBESTMATCHCOMMAND_H_

#include "abstractCommand.h"

/**
 * Compute the size of the largest darks set that matches T° distribution
 * with curerntly scanned lights
 */
class ComputeBestMatchCommand : public AbstractCommand {

	Q_OBJECT

public:
	ComputeBestMatchCommand();
	virtual ~ComputeBestMatchCommand();

	virtual void do_processing();

signals:

	/**
	 * A best match was found
	 * @param match the number of darks that best match lights T° distribution
	 */
	void bestMatchFound(int match);
};

#endif /* SRC_COMMANDS_COMPUTEBESTMATCHCOMMAND_H_ */
