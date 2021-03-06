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

#ifndef PLOTMANAGER_H
#define PLOTMANAGER_H

#include <QObject>

#include "../data/dataStore.h"
#include "../commands/signalDispatcher.h"
#include "../contrib/qCustomPlot/qcustomplot.h"

/**
 * @brief Manages all plots displayed in this app
 */
class PlotManager : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param darkTempPlot dark temperature evolution graph widget
     * @param darkTempDistriPlot dark temperature distribution graph widget
     * @param parent Qt parent
     */
    explicit PlotManager(QCustomPlot* darkTempPlot, QCustomPlot* darkTempDistriPlot, QObject *parent = 0);

private:
    QCustomPlot* _darkTempEvoPlot;
    QCustomPlot* _darkTempDistriPlot;

    void clearDarkTempEvoGraph();
    void refreshDarkTempEvoGraph();

    void clearDarkTempDistriGraph();
    void refreshDarkTempDistriGraph();

    int roundUp(int numToRound, int multiple);

signals:

private slots:
    /** dark model changed */
    void on_darkModelChanged();

    /** dark scan just started */
    void on_darkScanStarted();
};

#endif // PLOTMANAGER_H
