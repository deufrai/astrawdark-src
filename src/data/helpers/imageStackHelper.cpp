/*
 * imageStackHelper.cpp
 *
 *  Created on: 12 déc. 2015
 *      Author: deufrai
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
