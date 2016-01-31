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

#include "computeBestMatchCommand.h"

#include "data/dataStore.h"
#include "processing/darkMatcher.h"
#include "processing/exceptions/noDarkForShootSettingsExcpetion.h"
#include "processing/exceptions/noDarkForTempException.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ComputeBestMatchCommand::ComputeBestMatchCommand() {

	_description = tr("Compute best match");
}

ComputeBestMatchCommand::~ComputeBestMatchCommand() {

}

void ComputeBestMatchCommand::do_processing() {

	#ifndef QT_NO_DEBUG
		qDebug() << "Finding best match ...";
	#endif

	_message = tr("Finding best possible T° match...");
	emit statusChanged(this);

	/*
	 * We will do a dichotomic search of the biggest dark set
	 * having a T° distribution that matches our lights
	 */

	int low  = 0;
	int high = DataStore::getInstance()->getScannedDarks().size();
	int lowestBad = high;

	bool found = false;

	// we first try the biggest dar kset possible : all of them
	DarkMatcher matcher;


	do {

		try {

			#ifndef QT_NO_DEBUG
				qDebug() << "BEFORE : LOW = " << low << " | HIGH = " << high;
			#endif

			matcher.match(DataStore::getInstance()->getScannedLights(),
						  DataStore::getInstance()->getScannedDarks(),
						  high);

			low = high;
			high += (lowestBad - high) / 2;

			#ifndef QT_NO_DEBUG
				qDebug() << "AFTER : LOW = " << low << " | HIGH = " << high;
			#endif

			found = true;

		} catch (NoDarkForTempException const& e) {

			lowestBad = high;
			high -= (high - low ) / 2;

			#ifndef QT_NO_DEBUG
				qDebug() << "AFTER : LOW = " << low << " | HIGH = " << high;
			#endif

		} catch (NoDarkForShootSettingsExcpetion const& e) {

			_error = true;
			QString msg = tr("No darks match your shooting settings.");
			_reportMessages << msg;
			_commandReport.addSection(msg,QStringList());

			return;
		}

	} while ( high -low > 1 );


	if ( found ) {

		_message = tr("Best match found = %1").arg(high -1);
		emit statusChanged(this);
		_reportMessages << tr("OK.");
		_commandReport.addSection(tr("Completed successfully"),QStringList());


		#ifndef QT_NO_DEBUG
			qDebug() << "Best match found";
		#endif

	} else {

		_error = true;
		QString msg = tr("Couldn't find T° match");
		_reportMessages << msg;
		_commandReport.addSection(msg,QStringList());
	}
}

