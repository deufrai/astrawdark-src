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

#include "lightsCheckCommand.h"
#include "data/dto/imageInfo.h"
#include "data/dataStore.h"
#include "signalDispatcher.h"
#include <QList>


LightsCheckCommand::LightsCheckCommand()
{
    _description = QString(tr("Lights consistency check"));

    connect(this,
            &LightsCheckCommand::consistencyResult,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_consistencyResult);
}

void LightsCheckCommand::do_processing()
{
    QList<ImageInfo> lights = DataStore::getInstance()->getScannedLights();

    bool consistent = true;

    _message = tr("Checking lights...");
    emit statusChanged(this);

    ImageInfo reference, test;

    for ( int referenceIndex = 0; referenceIndex < lights.count() && consistent; ++referenceIndex) {

        reference = lights[referenceIndex];

        for ( int testIndex = referenceIndex +1; testIndex < lights.count() && consistent; ++testIndex ) {

            test = lights[testIndex];

            if ( ! ( reference.getCameraSerial()    == test.getCameraSerial()   &&
                     reference.getIso()             == test.getIso()            &&
                     reference.getExposure()        == test.getExposure()           ) ) {

                consistent = false;
            }
        }
    }

    if ( consistent ) {

        _reportMessages << tr("OK");
        _commandReport.addSection(tr("Completed successfully"),QStringList());

    } else {

        _warning = true;

        QString msg = tr("WARNING - you have mismatched lights in this folder");

        _reportMessages << msg;

        _commandReport.addSection(msg, QStringList() << tr("%1\nand\n%2\n")
                                  .arg(reference.getPath())
                                  .arg(test.getPath()).append(tr("don't have the same serial# + ISO + exposure time")));
    }

    _message = tr("All lights checked");
    emit statusChanged(this);
    emit consistencyResult(consistent);
}

