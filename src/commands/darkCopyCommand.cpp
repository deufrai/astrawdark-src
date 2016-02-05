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

#include "darkCopyCommand.h"
#include "data/dto/imageInfo.h"
#include "data/dataStore.h"

#include <QFileInfo>
#include <QStorageInfo>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DarkCopyCommand::DarkCopyCommand() {

	_description = tr("Copy darks");

}

DarkCopyCommand::~DarkCopyCommand() {


}

void DarkCopyCommand::do_processing() {

#ifndef QT_NO_DEBUG
	qDebug() << "DarkCopyCommand::do_processing -- START";
#endif

	// Get matched darks from dataStore
	QList<ImageInfo> darks = DataStore::getInstance()->getMatchedDarks();

	// get destination folder path
	QString copyPath = DataStore::getInstance()->getDarkCopyFolderPath();

	// get infos from the first dark, since they are all shot with same settings
	ImageInfo reference = darks.at(0);

	int expopsure   = reference.getExposure();
	int iso			= reference.getIso();

	int currentProgress = 0;
	emit progressMax(darks.size());

	QStringList copiedFilesReports;

	_message = tr("Checking free space...");
	emit statusChanged(this);
	
	// compute total data size
	qint64 totalSize = 0;

	foreach ( ImageInfo dark, darks ) {

		QFileInfo fileInfo(dark.getPath());
		totalSize += fileInfo.size();
	}

	#ifndef QT_NO_DEBUG
		qDebug() << "Total copy size : " << totalSize << " bytes";
	#endif

	QStorageInfo storageInfo(copyPath);

	#ifndef QT_NO_DEBUG
		qDebug() << "Destination available space : " << storageInfo.bytesAvailable() << " bytes";
	#endif

	if ( storageInfo.bytesAvailable() > totalSize ) {
	
		_message = tr("Matching darks...");
		emit statusChanged(this);
		
		foreach ( ImageInfo dark, darks ) {

			QFile sourceFile(dark.getPath());
			QFileInfo sourceInfo(sourceFile);
			int temperature = dark.getTemperature();

			QString destFileName = QString("%1_iso%2_%3sec_%4C.%5")
					.arg(currentProgress +1, 4, 10, QLatin1Char('0'))
					.arg(iso)
					.arg(expopsure)
					.arg(temperature)
					.arg(sourceInfo.completeSuffix());

			QString sourcePath = sourceInfo.filePath();
			QString destFilePath = copyPath + "/" + destFileName;

			_message = tr("Copying file %1/%2").arg(currentProgress +1 ).arg(darks.size());

			if ( sourceFile.copy( destFilePath) ) {

				copiedFilesReports << tr("Copied %1\nto %2\n").arg(sourcePath).arg(destFilePath);

			} else {

				_error = true;
				QString msg = tr("Error on file copy");

				QStringList errorDetails;
				errorDetails << tr("Could not copy %1\nto\n%2").arg(sourcePath).arg(destFilePath);

				_reportMessages << msg;
				_commandReport.addSection(msg, errorDetails);

	#ifndef QT_NO_DEBUG
				qDebug() << "Failed copy to " << destFilePath;
	#endif
				break;
			}

			emit progress(++currentProgress);
			emit statusChanged(this);
		}
		
	} else {
		
		_error = true;
		QString msg = tr("Not enought disk space");

		QStringList errorDetails;
		errorDetails << tr("There is not enought free disk space to copy all matched darks");

		_reportMessages << msg;
		_commandReport.addSection(msg, errorDetails);
	}

	if ( ! _error ) {

        _reportMessages << tr("OK");
        _commandReport.addSection(tr("Completed successfully"),copiedFilesReports);
	}

}
