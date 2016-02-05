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

#include "imageStackHelper.h"

ImageStackHelper::ImageStackHelper() {}

ImageStackHelper::~ImageStackHelper() {}

QList<ImageStack*> ImageStackHelper::createStackListFromImageInfos(QList<ImageInfo> infos) {

	QList<ImageStack*> stacks;

	foreach ( ImageInfo info, infos ) {

		bool found = false;

		foreach ( ImageStack* pStack, stacks ) {

			if ( ! found && info.getTemperature() == pStack->getTemperature() ) {

				pStack->add(info);
				found = true;
			}
		}

		if ( ! found ) {

			ImageStack* pStack = new ImageStack((info.getTemperature()));
			pStack->add(info);
			stacks.append(pStack);
		}
	}

	return stacks;
}
