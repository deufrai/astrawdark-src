/*
 * imageStackHelper.cpp
 *
 *  Created on: 12 déc. 2015
 *      Author: deufrai
 */

#include "imageStackHelper.h"

ImageStackHelper::ImageStackHelper() {}

ImageStackHelper::~ImageStackHelper() {}

QList<ImageStack> ImageStackHelper::createStackListFromImageInfos(QList<ImageInfo> infos) {

	QList<ImageStack> stacks;

	foreach ( ImageInfo info, infos ) {

		bool found = false;
		foreach ( ImageStack stack, stacks ) {

			if ( ! found && info.getTemperature() == stack.getTemperature() ) {

				stack.add(info);
				found = true;
			}
		}

		if ( ! found ) {

			ImageStack stack(info.getTemperature());
			stack.add(info);
			stacks.append(stack);
		}
	}

	return stacks;
}
