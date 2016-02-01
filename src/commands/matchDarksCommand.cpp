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

#include "matchDarksCommand.h"
#include "data/dataStore.h"
#include "data/dto/imageInfo.h"
#include "processing/darkMatcher.h"
#include "processing/exceptions/noDarkForShootSettingsExcpetion.h"
#include "processing/exceptions/noDarkForTempException.h"
#include "commands/signalDispatcher.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

MatchDarksCommand::MatchDarksCommand() {

	_description = QString(tr("Match darks"));

	connect(this,
			&MatchDarksCommand::matchFound,
			SignalDispatcher::getInstance(),
			&SignalDispatcher::matchFound);

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

	DarkMatcher matcher;

	_message = tr("Matching darks...");
	emit statusChanged(this);

	try {

		matcher.match(lights, allDarks, neededDarksCount);

		_message = tr("%1 darks matched").arg(matcher.getMatchedDarks().size());
        _reportMessages << tr("OK.");
        _commandReport.addSection(tr("Completed successfully"),QStringList());

		emit statusChanged(this);
		emit matchFound(matcher.getMatchedDarks());

	} catch ( NoDarkForShootSettingsExcpetion const& e ) {

        on_error(tr("ERROR - No darks available for your lights shooting settings"));


	} catch ( NoDarkForTempException const& e) {

        on_error(tr("ERROR - Not enough darks for T = %1°C. Needed = %2, Available = %3")
            .arg(e.getTemp())
            .arg(e.getNeeded())
            .arg(e.getAvailable()));
	}
}

void MatchDarksCommand::on_error(const QString msg) {

    _error = true;
    _reportMessages << msg;
    qWarning(msg.toStdString().c_str());
    _commandReport.addSection(msg, QStringList());
}
