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

#ifndef SRC_PROCESSING_EXCEPTIONS_NODARKFORTEMPEXCEPTION_H_
#define SRC_PROCESSING_EXCEPTIONS_NODARKFORTEMPEXCEPTION_H_

#include "matchingException.h"

/**
 * @brief Temperature matching failed because there are not
 *        enough darks for the given temperature
 */
class NoDarkForTempException : public MatchingException {

public:
	/**
	 * Constructor
	 *
	 * @param msg incorporated message
	 * @param temp temperature for the error
	 * @param needed how much darks we needed
	 * @param available how many darks are available
	 */
	NoDarkForTempException(const char* msg, int temp, int needed, int available);
	virtual ~NoDarkForTempException() throw ();

	/**
	 * get temperature
	 *
	 * @return temperature
	 */

	int getTemp() const { return _temp; }

	/**
	 * get needed darks count
	 *
	 * @return needed darks count
	 */
	int getNeeded() const { return _needed; }

	/**
	 * get available darks count
	 *
	 * @return available darks count
	 */
	int getAvailable() const { return _available; }

protected:
	/** temperature */
	int _temp;

	/** how many darks we needed */
	int _needed;

	/** how many darks are available */
	int _available;
};

#endif /* SRC_PROCESSING_EXCEPTIONS_NODARKFORTEMPEXCEPTION_H_ */
