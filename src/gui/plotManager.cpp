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
    _darkTempEvoPlot->addGraph();
    _darkTempEvoPlot->xAxis->setLabel(tr("Dark N°"));
    _darkTempEvoPlot->xAxis->setAutoTickStep(false);
    _darkTempEvoPlot->yAxis->setLabel(tr("Sensor temperature in °C"));
    _darkTempEvoPlot->yAxis->setAutoTickStep(false);

    _lightsTempEvoPlot->addGraph();
    _lightsTempEvoPlot->xAxis->setLabel(tr("Light N°"));
    _lightsTempEvoPlot->xAxis->setAutoTickStep(false);
    _lightsTempEvoPlot->yAxis->setLabel(tr("Sensor temperature in °C"));
    _lightsTempEvoPlot->yAxis->setAutoTickStep(false);

    _darkTempDistriPlot->addPlottable(new QCPBars(_darkTempDistriPlot->xAxis, _darkTempDistriPlot->yAxis));
    _darkTempDistriPlot->xAxis->setLabel(tr("Sensor temperature in C°"));
    _darkTempDistriPlot->xAxis->setAutoTickStep(false);
    _darkTempDistriPlot->xAxis->setAutoSubTicks(false);
    _darkTempDistriPlot->xAxis->setTickStep(1);
    _darkTempDistriPlot->xAxis->setTickLength(0);
    _darkTempDistriPlot->xAxis->setSubTickCount(0);
    _darkTempDistriPlot->yAxis->setLabel(tr("Dark count"));
    _darkTempDistriPlot->yAxis->setAutoTickStep(false);

    _lightsTempDistriPlot->addPlottable(new QCPBars(_lightsTempDistriPlot->xAxis, _lightsTempDistriPlot->yAxis));
    _lightsTempDistriPlot->xAxis->setLabel(tr("Sensor temperature in C°"));
    _lightsTempDistriPlot->xAxis->setAutoTickStep(false);
    _lightsTempDistriPlot->xAxis->setAutoSubTicks(false);
    _lightsTempDistriPlot->xAxis->setTickStep(1);
    _lightsTempDistriPlot->xAxis->setTickLength(0);
    _lightsTempDistriPlot->xAxis->setSubTickCount(0);
    _lightsTempDistriPlot->yAxis->setLabel(tr("Light count"));
    _lightsTempDistriPlot->yAxis->setAutoTickStep(false);


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
    if ( _darkTempEvoPlot->graphCount() > 0 ) {

        _darkTempEvoPlot->graph()->clearData();
        _darkTempEvoPlot->replot();
    }
}

void PlotManager::clearDarkTempDistriGraph()
{
    if ( _darkTempDistriPlot->plottableCount() ) {

        _darkTempDistriPlot->plottable()->clearData();
        _darkTempDistriPlot->replot();
    }
}

void PlotManager::clearLightsTempEvoGraph()
{
    if ( _lightsTempEvoPlot->graphCount()  > 0 ) {

        _lightsTempEvoPlot->graph()->clearData();
        _lightsTempEvoPlot->replot();
    }
}

void PlotManager::clearLightsTempDistriGraph()
{
    if ( _lightsTempDistriPlot->plottableCount() ) {

        _lightsTempDistriPlot->plottable()->clearData();
        _lightsTempDistriPlot->replot();
    }
}

void PlotManager::refreshDarkTempEvoGraph()
{
    QList<ImageInfo> unsortedData = DataStore::getInstance()->getFilteredDarks();

    // we sort darks by ascending date
    QMap<QString, ImageInfo> map;
    foreach (ImageInfo info, unsortedData) {

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

    _darkTempEvoPlot->graph()->setData(x,y);

    // set X axis range's maximum to be the next even number after max X value
    _darkTempEvoPlot->xAxis->setRange(0, (data.count()/2)*2+2);
    _darkTempEvoPlot->yAxis->setRange(0, maxTemp + 1);
    _darkTempEvoPlot->yAxis->setTickStep(roundUp(maxTemp / 10, 5));

    int xTickStep = data.count() / 20;

    if ( xTickStep < 1 ) {

        xTickStep = 1;
    }

    _darkTempEvoPlot->xAxis->setTickStep(xTickStep);

    _darkTempEvoPlot->replot();
}


void PlotManager::refreshDarkTempDistriGraph()
{
    QList<ImageInfo> darks = DataStore::getInstance()->getFilteredDarks();

    QVector<double> x, y;

    QMap<int, ImageInfo> data;
    foreach ( ImageInfo info, darks ) {

        data.insertMulti(info.getTemperature(), info);
    }

    int maxTemp = 0;
    int minTemp = 200;

    int maxCount = 0;

    foreach (int temperature, data.uniqueKeys()) {

        int count = data.values(temperature).count();

        x << temperature;
        y << count;

        if ( maxTemp < temperature ) maxTemp = temperature;
        if ( minTemp > temperature ) minTemp = temperature;

        if ( maxCount < count ) maxCount = count;
    }

    ((QCPBars*)(_darkTempDistriPlot->plottable(0)))->setData(x,y);

    _darkTempDistriPlot->xAxis->setRange(minTemp - 1, maxTemp + 1);
    _darkTempDistriPlot->yAxis->setRange(0, maxCount + 1);

    _darkTempDistriPlot->yAxis->setTickStep(roundUp(maxCount / 10, 5));

    _darkTempDistriPlot->replot();
}

void PlotManager::refreshLightsTempEvoGraph()
{
    QList<ImageInfo> unsortedData = DataStore::getInstance()->getScannedLights();

    // we sort frames by ascending date
    QMap<QString, ImageInfo> map;
    foreach (ImageInfo info, unsortedData) {

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

    _lightsTempEvoPlot->graph()->setData(x,y);

    // set X axis range's maximum to be the next even number after max X value
    _lightsTempEvoPlot->xAxis->setRange(0, (data.count()/2)*2+2);
    _lightsTempEvoPlot->yAxis->setRange(0, maxTemp + 1);
    _lightsTempEvoPlot->yAxis->setTickStep(roundUp(maxTemp / 10, 5));

    int xTickStep = data.count() / 20;

    if ( xTickStep < 1 ) {

        xTickStep = 1;
    }

    _lightsTempEvoPlot->xAxis->setTickStep(xTickStep);

    _lightsTempEvoPlot->replot();
}

void PlotManager::refreshLightsTempDistriGraph()
{
    QList<ImageInfo> lights = DataStore::getInstance()->getScannedLights();

    QVector<double> x, y;

    QMap<int, ImageInfo> data;
    foreach ( ImageInfo info, lights ) {

        data.insertMulti(info.getTemperature(), info);
    }

    int maxTemp = 0;
    int minTemp = 200;

    int maxCount = 0;

    foreach (int temperature, data.uniqueKeys()) {

        int count = data.values(temperature).count();

        x << temperature;
        y << count;

        if ( maxTemp < temperature ) maxTemp = temperature;
        if ( minTemp > temperature ) minTemp = temperature;

        if ( maxCount < count ) maxCount = count;
    }

    ((QCPBars*)(_lightsTempDistriPlot->plottable(0)))->setData(x,y);

    _lightsTempDistriPlot->xAxis->setRange(minTemp - 1, maxTemp + 1);
    _lightsTempDistriPlot->yAxis->setRange(0, maxCount + 1);

    _lightsTempDistriPlot->yAxis->setTickStep(roundUp(maxCount / 10, 5));

    _lightsTempDistriPlot->replot();
}

int PlotManager::roundUp(int numToRound, int multiple)
{

    if(multiple == 0) return numToRound;

    int remainder = numToRound % multiple;

    if (numToRound != 0 && remainder == 0) return numToRound;

    int nRet = numToRound + multiple - remainder;

    if ( nRet < multiple ) nRet = 1;

    return nRet;

}
