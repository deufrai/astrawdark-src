/*
 * matchDarksCommand.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: deufrai
 */

#include "matchDarksCommand.h"
#include "data/dataStore.h"
#include "data/dto/imageInfo.h"
#include "data/helpers/imageStackHelper.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

MatchDarksCommand::MatchDarksCommand() {

	_description = QString(tr("Match darks"));

}

MatchDarksCommand::~MatchDarksCommand() {


}

void MatchDarksCommand::do_processing() {

	// Get needed darks count
	int neededDarksCount = DataStore::getInstance()->getNeededDarks4Matching();

	// Get lights
	QList<ImageInfo> lights = DataStore::getInstance()->getScannedLights();

	// Get all darks from library
	QList<ImageInfo> allDarks = DataStore::getInstance()->getScannedDarks();

	#ifndef QT_NO_DEBUG
		qDebug() << "Matching " << neededDarksCount << " darks";
	#endif

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

	_message = tr("Filtering darks...");
	emit statusChanged(this);

	QList<ImageInfo> filteredDarks;

	foreach (ImageInfo dark, allDarks) {

		if ( dark.getCameraSerial() == serial       &&
			 dark.getExposure()     == exposureTime &&
			 dark.getIso()          == isoSpeed        ) {

			filteredDarks.append(dark);
		}
	}

	_message = tr("Darks filtered");
	emit statusChanged(this);

	if ( filteredDarks.isEmpty() ) {

		qWarning("No darks match your lights shooting conditions");

        _error = true;
        QString msg = tr("ERROR - No darks match your lights shooting conditions");
        _reportMessages << msg;
        _commandReport.addSection(msg, QStringList());

	} else {

		#ifndef QT_NO_DEBUG
			qDebug() << neededDarksCount << " darks wanted. We found "
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
					int wantedDarks = lightStackSize * neededDarksCount / lights.size();

					if ( 0 == wantedDarks ) {

						wantedDarks = 1;
					}

					#ifndef QT_NO_DEBUG
					qDebug() << lightStackTemp << "°C :  "
							 << wantedDarks
							 << "\t|   "
							 << darkStackSize;
					#endif

					if ( wantedDarks > darkStackSize ) {

				        _error = true;
				        QString msg = tr("ERROR - Not enough darks for T° = %1").arg(lightStackTemp);

						#ifndef QT_NO_DEBUG
							qDebug(msg.toStdString().c_str());
						#endif

				        _reportMessages << msg;
				        _commandReport.addSection(msg, QStringList());
				        return;

					} else {

						matchedDarksCount += wantedDarks;
					}
				}
			}

			if ( ! foundDarkStack ) {

		        _error = true;
		        QString msg = tr("ERROR - Not enough darks for T° = %1").arg(lightStackTemp);

		        #ifndef QT_NO_DEBUG
		        	qDebug(msg.toStdString().c_str());
				#endif

		        _reportMessages << msg;
		        _commandReport.addSection(msg, QStringList());
		        return;
			}
		}

		#ifndef QT_NO_DEBUG
		qDebug() << "Matched " << matchedDarksCount << " darks";
		#endif
	}
}
