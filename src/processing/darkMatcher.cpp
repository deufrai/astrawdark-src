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

#include "darkMatcher.h"
#include "data/helpers/imageStackHelper.h"

#include <stdexcept>

#include <QObject>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DarkMatcher::DarkMatcher() {


}

DarkMatcher::~DarkMatcher() {

}

void DarkMatcher::match(QList<ImageInfo> lights, QList<ImageInfo> darks, int desiredDarkCount) {

	/*
	 * Here we know that all lights have been checked for consistancy
	 * so we can get camera body serial number, exposure time and ISO settings
	 * from whichever light
	 */
	ImageInfo referenceLight = lights.at(0);

	QString serial = referenceLight.getCameraSerial();
	int exposureTime = referenceLight.getExposure();
	int isoSpeed = referenceLight.getIso();

	/*
	 * We filter darks based on these shooting conditions
	 */
	QList<ImageInfo> filteredDarks;

	foreach (ImageInfo dark, darks) {

		if ( dark.getCameraSerial() == serial       &&
			 dark.getExposure()     == exposureTime &&
			 dark.getIso()          == isoSpeed        ) {

			filteredDarks.append(dark);
		}
	}

	if ( filteredDarks.isEmpty() ) {

		throw std::runtime_error(QObject::tr("ERROR - No darks match your lights shooting conditions").toStdString());

	} else {

		#ifndef QT_NO_DEBUG
			qDebug() << desiredDarkCount << " darks wanted. We found "
					 << filteredDarks.size() << " darks that match shooting settings";

			qDebug() << "  T°  : needed\t| available";
			qDebug() << "=================================";
		#endif

		QList<ImageStack*> lightStacks = ImageStackHelper::createStackListFromImageInfos(lights);
		QList<ImageStack*> darkStacks = ImageStackHelper::createStackListFromImageInfos(filteredDarks);

		int matchedDarksCount = 0;

		foreach( ImageStack* pLightStack, lightStacks ) {

			int lightStackTemp = pLightStack->getTemperature();
			int lightStackSize = pLightStack->getSize();

			bool foundDarkStack = false;

			foreach ( ImageStack* pDarkStack, darkStacks ) {

				if ( pDarkStack->getTemperature() == lightStackTemp ) {

					foundDarkStack = true;

					int darkStackSize = pDarkStack->getSize();
					int neededDarks = lightStackSize * desiredDarkCount / lights.size();

					if ( 0 == neededDarks ) {

						neededDarks = 1;
					}

					#ifndef QT_NO_DEBUG
					qDebug() << lightStackTemp << "°C :  "
							 << neededDarks
							 << "\t|   "
							 << darkStackSize;
					#endif

					if ( neededDarks > darkStackSize ) {

						throw std::runtime_error(QObject::tr("ERROR - Not enough darks for T° = %1")
							.arg(lightStackTemp)
							.toStdString());

					} else {

						matchedDarksCount += neededDarks;
						// TODO : feed actual result member
					}
				}
			}

			if ( ! foundDarkStack ) {

				throw std::runtime_error(QObject::tr("ERROR - No dark for T° = %1")
					.arg(lightStackTemp)
					.toStdString());
			}
		}

		#ifndef QT_NO_DEBUG
		qDebug() << "Matched " << matchedDarksCount << " darks";
		#endif
	}
}
