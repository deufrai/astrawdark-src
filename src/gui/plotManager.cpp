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
#include "data/helpers/imageStackHelper.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

PlotManager::PlotManager(QCustomPlot* darkTempPlot,
                         QCustomPlot* darkTempDistriPlot,
                         QCustomPlot* lightsTempPlot,
                         QCustomPlot* lightsTempDistriPlot,
                         QObject *parent)
    : QObject(parent),
      _darkTempEvoPlot(darkTempPlot),
      _darkTempDistriPlot(darkTempDistriPlot),
      _lightsTempEvoPlot(lightsTempPlot),
      _lightsTempDistriPlot(lightsTempDistriPlot)

{
    setupTempEvoPlot(_darkTempEvoPlot, tr("Dark N°"), tr("Sensor temperature in °C"));
    setupTempEvoPlot(_lightsTempEvoPlot, tr("Light N°"), tr("Sensor temperature in °C"));

    setupTempDisrtiPlot(_darkTempDistriPlot, tr("Sensor temperature in C°"), tr("Dark count"));
    setupTempDisrtiPlot(_lightsTempDistriPlot, tr("Sensor temperature in C°"), tr("Light count"));

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkListModelChanged,
            this,
            &PlotManager::on_darkModelChanged);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanStarted,
            this,
            &PlotManager::on_darkScanStarted);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::lightsScanStarted,
            this,
            &PlotManager::on_lightsScanStarted);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::lightsScanDone,
            this,
            &PlotManager::on_lightsScanDone);
}

void PlotManager::on_darkModelChanged()
{
    refreshDarkTempEvoGraph();
    refreshDarkTempDistriGraph();
}

void PlotManager::on_darkScanStarted()
{
    clearDarkTempEvoGraph();
    clearDarkTempDistriGraph();
}

void PlotManager::on_lightsScanDone()
{
    refreshLightsTempEvoGraph();
    refreshLightsTempDistriGraph();
}

void PlotManager::on_lightsScanStarted()
{
    clearLightsTempEvoGraph();
    clearLightsTempDistriGraph();
}

void PlotManager::clearDarkTempEvoGraph()
{
    clearPlot(_darkTempEvoPlot);
}

void PlotManager::clearDarkTempDistriGraph()
{
    clearPlot(_darkTempDistriPlot);
}

void PlotManager::clearLightsTempEvoGraph()
{
    clearPlot(_lightsTempEvoPlot);
}

void PlotManager::clearLightsTempDistriGraph()
{
    clearPlot(_lightsTempDistriPlot);
}

void PlotManager::refreshDarkTempEvoGraph()
{
    refreshTempEvoPlot(_darkTempEvoPlot, DataStore::getInstance()->getFilteredDarks());
}


void PlotManager::refreshDarkTempDistriGraph()
{
    refreshTempDistriPlot(_darkTempDistriPlot, DataStore::getInstance()->getFilteredDarks());
}

void PlotManager::refreshLightsTempEvoGraph()
{
    refreshTempEvoPlot(_lightsTempEvoPlot, DataStore::getInstance()->getScannedLights());
}

void PlotManager::refreshLightsTempDistriGraph()
{
    refreshTempDistriPlot(_lightsTempDistriPlot, DataStore::getInstance()->getScannedLights());
}

void PlotManager::setupTempEvoPlot(QCustomPlot *plot, QString xLabel, QString yLabel)
{
    plot->addGraph();

    plot->xAxis->setLabel(xLabel);
    plot->xAxis->setAutoTickStep(false);

    plot->yAxis->setLabel(yLabel);
}

void PlotManager::setupTempDisrtiPlot(QCustomPlot *plot, QString xLabel, QString yLabel)
{
    plot->addPlottable(new QCPBars(plot->xAxis, plot->yAxis));

    plot->xAxis->setLabel(xLabel);
    plot->xAxis->setAutoTickStep(false);
    plot->xAxis->setAutoSubTicks(false);
    plot->xAxis->setTickStep(1);
    plot->xAxis->setTickLength(0);
    plot->xAxis->setSubTickCount(0);

    plot->yAxis->setLabel(yLabel);
}

void PlotManager::refreshTempEvoPlot(QCustomPlot *plot, QList<ImageInfo> infos)
{
    // we sort frames by ascending date
    QMap<QString, ImageInfo> map;
    foreach (ImageInfo info, infos) {

        map.insert(info.getDate(), info);
    }

    QList<ImageInfo> data;
    foreach ( QString date, map.keys() ) {

        data << map.value(date);
    }

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

    plot->graph()->setData(x,y);

    // set X axis range's maximum to be the next even number after max X value
    plot->xAxis->setRange(0, (data.count()/2)*2+2);
    plot->yAxis->setRange(0, maxTemp + 1);
    plot->yAxis->setTickStep(roundUp(maxTemp / 10, 5));

    int xTickStep = data.count() / 20;

    if ( xTickStep < 1 ) {

        xTickStep = 1;
    }

    plot->xAxis->setTickStep(xTickStep);

    // ensure data visibility on small datasets
    if ( data.count() < 5 ) {

        _darkTempEvoPlot->graph()->setScatterStyle(QCPScatterStyle::ssCross);

    } else {

        _darkTempEvoPlot->graph()->setScatterStyle(QCPScatterStyle::ssNone);
    }

    plot->replot();
}

void PlotManager::refreshTempDistriPlot(QCustomPlot *plot, QList<ImageInfo> infos)
{
    QVector<double> x, y;

    QList<ImageStack*> stacks = ImageStackHelper::createStackListFromImageInfos(infos);

    int maxTemp = 0;
    int minTemp = 200;

    int maxCount = 0;

    foreach ( ImageStack* stack, stacks ) {

        int count = stack->getSize();
        int temperature = stack->getTemperature();

        x << temperature;
        y << count;

        if ( maxTemp < temperature ) {

            maxTemp = temperature;
        }

        if ( minTemp > temperature ) {

            minTemp = temperature;
        }

        if ( maxCount < count ) {

            maxCount = count;
        }
    }

    static_cast<QCPBars*>(plot->plottable(0))->setData(x,y);

    // ARD-100: fix X axis when dataset is empty
    if ( infos.count() == 0 ) {

        minTemp = 1;
        maxTemp = 0;
    }

    plot->xAxis->setRange(minTemp - 1, maxTemp + 1);
    plot->yAxis->setRange(0, maxCount + 1);

    plot->yAxis->setTickStep(roundUp(maxCount / 10, 5));

    plot->replot();
}

void PlotManager::clearPlot(QCustomPlot *plot)
{
    if ( plot->plottableCount() ) {

        plot->plottable()->clearData();
        plot->replot();
    }
}

int PlotManager::roundUp(int numToRound, int multiple)
{

    if(multiple == 0) {

        return numToRound;
    }

    int remainder = numToRound % multiple;

    if (numToRound != 0 && remainder == 0) {

        return numToRound;
    }

    int nRet = numToRound + multiple - remainder;

    if ( nRet < multiple ) {

        nRet = 1;
    }

    return nRet;

}
