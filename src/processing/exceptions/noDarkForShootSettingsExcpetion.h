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

#ifndef SRC_PROCESSING_EXCEPTIONS_NODARKFORSHOOTSETTINGSEXCPETION_H_
#define SRC_PROCESSING_EXCEPTIONS_NODARKFORSHOOTSETTINGSEXCPETION_H_

#include "matchingException.h"

/**
 * @brief Temperature matching failed because of shooting settings mismsatch
 *        between lights and darks.
 */
class NoDarkForShootSettingsExcpetion : public MatchingException {

public:
	/**
	 * Constructor
	 *
	 * @param msg incorporated message
	 */
	NoDarkForShootSettingsExcpetion(const char* msg);

	virtual ~NoDarkForShootSettingsExcpetion() throw ();
};

#endif /* SRC_PROCESSING_EXCEPTIONS_NODARKFORSHOOTSETTINGSEXCPETION_H_ */
