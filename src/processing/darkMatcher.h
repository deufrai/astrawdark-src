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

#ifndef SRC_PROCESSING_DARKMATCHER_H_
#define SRC_PROCESSING_DARKMATCHER_H_

#include "data/dataStore.h"
#include "data/dto/imageInfo.h"

#include <QList>

/**
 * @brief In charge of the actual dark temperature matching.
 *
 */
class DarkMatcher {

public:
	DarkMatcher();
	virtual ~DarkMatcher();

	/**
	 * Get processing results
	 *
	 * @return a list of ImageInfo
	 */
	QList<ImageInfo> getMatchedDarks() const { return _results; }

	/**
	 * perform dark matching
	 *
	 * @param lights : list of lights to be preprocessed
	 * @param darks : list of all darks in library
	 * @param desiredDarkCount : how many darks must we use for matching
	 */
	void match(QList<ImageInfo> lights, QList<ImageInfo> darks, int desiredDarkCount);

private:
	QList<ImageInfo> _results;


};

#endif /* SRC_PROCESSING_DARKMATCHER_H_ */
