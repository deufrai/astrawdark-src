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
#include "data/helpers/imageStackHelper.h"
#include "processing/darkMatcher.h"

#include <exception>

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

	DarkMatcher matcher;

	try {

		matcher.match(lights, allDarks, neededDarksCount);

	} catch ( std::exception const& e) {

        _error = true;
        //TODO : get localized error messages
        QString msg = e.what();
        _reportMessages << msg;
        _commandReport.addSection(msg, QStringList());

	}

}
