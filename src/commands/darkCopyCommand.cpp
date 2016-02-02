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

	// compute total data size
	qint64 totalSize = 0;

	foreach ( ImageInfo dark, darks ) {

		QFileInfo fileInfo(dark.getPath());
		totalSize += fileInfo.size();
	}

	#ifndef QT_NO_DEBUG
		qDebug() << "Total copy size : " << totalSize << " bytes";
	#endif

	// Check filesystem free space
	QString copyPath = DataStore::getInstance()->getDarkCopyFolderPath();
	QStorageInfo storageInfo(copyPath);

	#ifndef QT_NO_DEBUG
		qDebug() << "Destination available space : " << storageInfo.bytesAvailable() << " bytes";
	#endif

	if ( storageInfo.bytesAvailable() > totalSize ) {

		// TODO: check if destination is empty

		int currentProgress = 0;
		emit progressMax(darks.size());

		foreach ( ImageInfo dark, darks ) {

			QFile sourceFile(dark.getPath());
			QFileInfo sourceInfo(sourceFile);

			if ( sourceFile.copy(copyPath + "/" + sourceInfo.fileName() ) ) {

				emit progress(++currentProgress);
				_message = tr("Copied %1/%2 file(s)").arg(currentProgress).arg(darks.size());
				#ifndef QT_NO_DEBUG
					qDebug() << "Successfully copied to " << copyPath + "/" + sourceInfo.fileName();
				#endif

			} else {

				//TODO: gracefully abort
				_error = true;
				#ifndef QT_NO_DEBUG
					qDebug() << "Failed copy to " << copyPath + "/" + sourceInfo.fileName();
				#endif
				break;
			}

			emit statusChanged(this);
		}

	} else {

		//TODO: gracefully abort
		_error = true;
		#ifndef QT_NO_DEBUG
			qDebug() << "Not enough space on device for copy";
		#endif
	}

	// copy files
}
