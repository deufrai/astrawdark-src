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

#include "plotManager.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

PlotManager::PlotManager(QCustomPlot *darkTempPlot, QObject *parent)
    : QObject(parent),
      _darkTempPlot(darkTempPlot)

{
    _darkTempPlot->addGraph();

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkListModelChanged,
            this,
            &PlotManager::on_darkModelChanged);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanStarted,
            this,
            &PlotManager::on_darkScanStarted);
}

void PlotManager::on_darkModelChanged()
{
    refreshDarkTempPlot();
}

void PlotManager::on_darkScanStarted()
{
    clearDarkTempGraph();
}

void PlotManager::clearDarkTempGraph()
{
    if ( _darkTempPlot->graphCount() > 0 ) {

        _darkTempPlot->graph()->clearData();
        _darkTempPlot->replot();
    }
}

void PlotManager::refreshDarkTempPlot()
{
    clearDarkTempGraph();

    QList<ImageInfo> data = DataStore::getInstance()->getFilteredDarks();

    QVector<double> x, y;

    int counter = 0;
    int maxTemp = 0;
    foreach (ImageInfo info, data) {

        x << counter++;
        y << info.getTemperature();

        if ( info.getTemperature() > maxTemp ) {

            maxTemp = info.getTemperature();
        }
    }

    _darkTempPlot->graph()->setData(x,y);

    _darkTempPlot->xAxis->setLabel("Dark N°");
    _darkTempPlot->yAxis->setLabel("Sensor temperature in °C");

    // set X axis range's maximum to be the next even number after max X value
    _darkTempPlot->xAxis->setRange(0, (data.count()/2)*2+2);
    _darkTempPlot->yAxis->setRange(0, maxTemp);

    _darkTempPlot->replot();
}
