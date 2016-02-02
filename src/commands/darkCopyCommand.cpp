/*
 * darkCopyCommand.cpp
 *
 *  Created on: 2 févr. 2016
 *      Author: deufrai
 */

#include "darkCopyCommand.h"
#include "data/dto/imageInfo.h"
#include "data/dataStore.h"

#include <QFileInfo>

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

	int currentProgress = 0;
	emit progressMax(darks.size());

	QStringList copiedFilesReports;

	foreach ( ImageInfo dark, darks ) {

		QFile sourceFile(dark.getPath());
		QFileInfo sourceInfo(sourceFile);
		QString sourcePath = sourceInfo.filePath();
		QString destPath = copyPath + "/" + sourceInfo.fileName();

		_message = tr("Copying file %1/%2").arg(currentProgress +1 ).arg(darks.size());

		if ( sourceFile.copy( destPath) ) {

			copiedFilesReports << tr("Copied %1\nto %2\n").arg(sourcePath).arg(destPath);

		} else {

			_error = true;
			QString msg = tr("Error on file copy");

			QStringList errorDetails;
			errorDetails << tr("Could not copy %1\nto\n%2").arg(sourcePath).arg(destPath);

			_reportMessages << msg;
	        _commandReport.addSection(msg, errorDetails);

#ifndef QT_NO_DEBUG
			qDebug() << "Failed copy to " << copyPath + "/" + sourceInfo.fileName();
#endif
			break;
		}

		emit progress(++currentProgress);
		emit statusChanged(this);
	}

	if ( ! _error ) {

        _reportMessages << tr("OK");
        _commandReport.addSection(tr("Completed successfully"),copiedFilesReports);
	}

}
