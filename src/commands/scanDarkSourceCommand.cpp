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

#include "scanDarkSourceCommand.h"
#include "signalDispatcher.h"

#include <QStringList>
#include <QFileInfoList>
#include <QDir>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

ScanDarkSourceCommand::ScanDarkSourceCommand(const QStringList &sources)
    : AbstractScanCommand::AbstractScanCommand(sources)
{
    _description = QString(tr("Dark library scan"));

    connect(this,
            &ScanDarkSourceCommand::scanStarted,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_darkScanStarted);

    connect(this,
            &ScanDarkSourceCommand::scanDone,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_darkScanDone);
}

void ScanDarkSourceCommand::checkForErrors()
{
    /*
     * If dark sources were missing, update error message
     */
    if ( ! _missingDirsPaths.isEmpty() ) {

        _warning = true;

        QString msg = tr("WARNING - Ignored %1 missing dark source folder")
                .arg(_missingDirsPaths.count());

        _reportMessages << msg;

        _commandReport.addSection(msg, _missingDirsPaths);
    }

    AbstractScanCommand::checkForErrors();
}

void ScanDarkSourceCommand::do_processing()
{
    emit scanStarted();

    if ( ! _sources.isEmpty() ) {

        foreach (QString path, _sources) {

            /*
             * If current dark source folder is missing,
             * we skip it and store its path for error display
             */
            if ( ! QDir(path).exists() ) {

                _missingDirsPaths << path;
                continue;
            }

            getRawPathsInDirectory(path);
        }

        retrieveExifMetadata();

        /*
         * If we saw short frames, update warnings & errors
         */
        checkForErrors();

        // tell the world we're done
        emit scanDone(_imageInfos);
    }
}

