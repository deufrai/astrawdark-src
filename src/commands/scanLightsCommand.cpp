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

#include "scanLightsCommand.h"
#include "signalDispatcher.h"

#include <QDir>

ScanLightsCommand::ScanLightsCommand(const QString dir)
    : AbstractScanCommand(QStringList() << dir)
{
    _description = QString(tr("Lights folder scan"));

    connect(this,
            &ScanLightsCommand::scanStarted,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_lightsScanStarted);

    connect(this,
            &ScanLightsCommand::scanDone,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_lightsScanDone);
}

void ScanLightsCommand::do_processing()
{
    emit scanStarted();

    QString dir = _sources.at(0);

    if ( QDir(dir).exists() ) {

        getRawPathsInDirectory(dir);

        retrieveExifMetadata();

        checkForErrors();

        emit scanDone(_imageInfos);
    }
}

