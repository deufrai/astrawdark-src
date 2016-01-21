/*
 * matchDarksCommand.cpp
 *
 *  Created on: 21 janv. 2016
 *      Author: deufrai
 */

#include "matchDarksCommand.h"
#include "data/dataStore.h"
#include "data/dto/imageInfo.h"

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
	 * do we can get camera body serial number, exposure time and ISO settings
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

		qWarning() << "No darks match your lights shooting conditions";

        _error = true;
        QString msg = tr("ERROR - No darks match your lights shooting conditions");
        _reportMessages << msg;
        _commandReport.addSection(msg, QStringList());

	} else {

		#ifndef QT_NO_DEBUG
			qDebug() << neededDarksCount << " darks wanted. We found "
					 << filteredDarks.size() << " darks that match shooting settings";
		#endif


	}
}
