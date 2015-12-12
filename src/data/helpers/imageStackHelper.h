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

#ifndef SRC_DATA_HELPERS_IMAGESTACKHELPER_H_
#define SRC_DATA_HELPERS_IMAGESTACKHELPER_H_

#include <QList>

#include "data/dto/imageStack.h"

/**
 * @brief provides static helpers for ImageStack creation and manipulation
 *
 */
class ImageStackHelper {

private:
	ImageStackHelper();
	virtual ~ImageStackHelper();

	/**
	 * Create a list of ImageStack from a list of ImageInfo based
	 */
	static QList<ImageStack> createStackListFromImageInfos(QList<ImageInfo> infos );

};

#endif /* SRC_DATA_HELPERS_IMAGESTACKHELPER_H_ */
