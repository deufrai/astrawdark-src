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
#include "processing/exceptions/noDarkForShootSettingsExcpetion.h"
#include "processing/exceptions/noDarkForTempException.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DarkMatcher::DarkMatcher() {


}

DarkMatcher::~DarkMatcher() {

}

void DarkMatcher::match(QList<ImageInfo> lights, QList<ImageInfo> darks, int desiredDarkCount) {

	_results.clear();

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
	 * We filter darks based on these shooting conditions.
	 * Keeping only darks shot with :
	 *
	 *  - same camera body, based on serial #
	 *  - same exposure time
	 *  - same ISO settings
	 */
	QList<ImageInfo> filteredDarks;

	foreach (ImageInfo dark, darks) {

		if ( dark.getCameraSerial() == serial       &&
				dark.getExposure()  == exposureTime &&
				dark.getIso()       == isoSpeed        ) {

			filteredDarks.append(dark);
		}
	}

	if ( ! filteredDarks.isEmpty() ) {

		#ifndef QT_NO_DEBUG
				qDebug() << desiredDarkCount << " darks wanted. We found "
						<< filteredDarks.size() << " darks that match shooting settings";
		#endif

		if ( desiredDarkCount > filteredDarks.size() ) {

			throw NoDarkForTempException("Not enough darks", 0, desiredDarkCount, filteredDarks.size());
		}

		#ifndef QT_NO_DEBUG
				qDebug() << "  T°  : needed\t| available";
				qDebug() << "=================================";
		#endif

		/*
		 * Create temperature stacks for lights & filtered darks
		 */
		QList<ImageStack*> lightStacks = ImageStackHelper::createStackListFromImageInfos(lights);
		QList<ImageStack*> darkStacks = ImageStackHelper::createStackListFromImageInfos(filteredDarks);

		/*
		 * For each light stack
		 */
		foreach( ImageStack* pLightStack, lightStacks ) {

			int lightStackSize = pLightStack->getSize();

			bool foundDarkStack = false;

			/*
			 * compute how many darks we need for this temperature
			 *
			 *
			 *                                           total number of darks wanted
			 * needed darks = size of lights stack x  ---------------------------------
			 *                                             total number of lights
			 *
			 * FIXME: rounding leads to matched dark count beeing slightly off by +- 1
			 */
			int neededDarks = qRound(lightStackSize * desiredDarkCount / static_cast<double> (lights.size()));

			/*
			 * we look for a dark stack with same temperature
			 */
			int lightStackTemp = pLightStack->getTemperature();

			foreach ( ImageStack* pDarkStack, darkStacks ) {

				if ( pDarkStack->getTemperature() == lightStackTemp ) {

					/*
					 * Found the right dark stack  !!!
					 */
					foundDarkStack = true;

					int darkStackSize = pDarkStack->getSize();

					#ifndef QT_NO_DEBUG
						qDebug() << lightStackTemp << "°C :  "
								<< neededDarks
								<< "\t|   "
								<< darkStackSize;
					#endif

					if ( neededDarks <= darkStackSize ) {

						/*
						 * Darks stack has enough images, we feed the monkey
						 */
						for ( int i=0; i<neededDarks; i++ ) {

							_results << pDarkStack->getImages().at(i);
						}

					} else {

						#ifndef QT_NO_DEBUG
							qDebug() << "Not enough darks for T = " << lightStackTemp << " °C";
						#endif
						throw NoDarkForTempException("Not enough darks", lightStackTemp, neededDarks, darkStackSize);
					}
				}
			}

			if ( ! foundDarkStack ) {

				#ifndef QT_NO_DEBUG
					qDebug() << "No darks for T = " << lightStackTemp << " °C";
				#endif
				throw NoDarkForTempException("Not enough darks", lightStackTemp, neededDarks, 0);
			}
		}

		#ifndef QT_NO_DEBUG
				qDebug() << "Matched " << _results.size() << " darks";
		#endif


	} else {

		throw NoDarkForShootSettingsExcpetion("No darks match your lights shooting conditions");
	}
}
