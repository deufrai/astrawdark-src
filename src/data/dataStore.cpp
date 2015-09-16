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

#include "dataStore.h"
#include "imageInfo.h"
#include "../globals.h"
#include "../commands/signalDispatcher.h"

#include <QSettings>
#include <QTime>
#include <QtConcurrent>


#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DataStore* DataStore::_instance = 0;

QString DataStore::_S_DarkDisplayFilter = "";

DataStore* DataStore::getInstance() {

    if ( 0 == _instance ) _instance = new DataStore();

    return _instance;
}

DataStore::DataStore()
    : _darkListModel(new QStandardItemModel()),
      _commandListModel(new QStandardItemModel()),
      _darkTreeModel(new QStandardItemModel()),
      _rememberWindowGeometry(false),
      _scanDarksOnStartup(false)
{
    QSettings settings;

    /*
     * Load values from stored preferences
     */
    if ( settings.contains(Globals::SETTINGKEY_DARK_SOURCES) ) {

        _darkSources = settings.value(Globals::SETTINGKEY_DARK_SOURCES).toStringList();
    }

    if ( settings.contains(Globals::SETTINGKEY_WINDOW_GEOMETRY_REMEMBER) ) {

        _rememberWindowGeometry = settings.value(Globals::SETTINGKEY_WINDOW_GEOMETRY_REMEMBER).toBool();
    }

    if ( settings.contains(Globals::SETTINGKEY_SCANDARKS_ON_STARTUP) ) {

        _scanDarksOnStartup = settings.value(Globals::SETTINGKEY_SCANDARKS_ON_STARTUP).toBool();
    }

    /*
     * Model setup
     */
    _commandListModel->setColumnCount(6);
    _commandListModel->setHorizontalHeaderLabels(QStringList()
                                                 << tr("N°")
                                                 << tr("Time")
                                                 << tr("Status")
                                                 << tr("Command")
                                                 << tr("Progress")
                                                 << tr("Report"));

    _darkListModel->setColumnCount(6);
    _darkListModel->setHorizontalHeaderLabels(QStringList()
                                              << tr("Path")
                                              << tr("Model")
                                              << tr("Exposure\n(sec.)")
                                              << tr("ISO")
                                              << tr("Date")
                                              << tr ("Temperature\n(°C)"));

    _darkTreeModel->setColumnCount(1);
    _darkTreeModel->setHorizontalHeaderLabels(QStringList() << tr("Dark families"));

    // signaling
    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanStarted,
            this,
            &DataStore::on_newDarkScanStarted);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanDone,
            this,
            &DataStore::on_newDarkScanResult);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::commandCreated,
            this,
            &DataStore::on_CommandCreated);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::commandStatusChange,
            this,
            &DataStore::on_CommandStatusChange);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkSourcesChanged,
            this,
            &DataStore::on_newDarkSources);

    connect(this,
            &DataStore::darkListModelChanged,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_darkListModelChanged);

}

void DataStore::populateDarkListModel(QList<ImageInfo> darks)
{
    _darkListModel->setRowCount(0);
    _darkListModel->setRowCount(darks.count());

    int row = 0;

    foreach (ImageInfo info, darks) {

        _darkListModel->setData(_darkListModel->index(row, 0, QModelIndex()),
                                info.getPath());

        _darkListModel->setData(_darkListModel->index(row, 1, QModelIndex()),
                                info.getModel());

        _darkListModel->setData(_darkListModel->index(row, 2, QModelIndex()),
                                info.getExposure());

        _darkListModel->setData(_darkListModel->index(row, 3, QModelIndex()),
                                info.getIso());

        _darkListModel->setData(_darkListModel->index(row, 4, QModelIndex()),
                                info.getDate());

        _darkListModel->setData(_darkListModel->index(row, 5, QModelIndex()),
                                info.getTemperature() == ImageInfo::UNDEFINED?
                                    "":
                                    QString::number(info.getTemperature()));

        ++row;

    }

    _darkListModel->sort(4);

    emit darkListModelChanged();
}

bool DataStore::filterDark(ImageInfo dark)
{
    if ( _S_DarkDisplayFilter.isEmpty() ) return true;

    QStringList filters = _S_DarkDisplayFilter.split('|');

    QString  serial = "";
    int     iso = -1;
    int     expo = -1;

    switch (filters.count()) {

    case 3:
        expo = filters.at(2).toInt();
        iso = filters.at(1).toInt();
        serial = filters.at(0);

        return dark.getCameraSerial() == serial &&
                dark.getIso() == iso             &&
                dark.getExposure() == expo;

        break;

    case 2:
        iso = filters.at(1).toInt();
        serial = filters.at(0);

        return dark.getCameraSerial() == serial &&
                dark.getIso() == iso;

        break;

    case 1:
        serial = filters.at(0);

        return dark.getCameraSerial() == serial;
        break;

    default:
        return true;
        break;
    }
}

void DataStore::on_newDarkScanResult(QList<ImageInfo> darks)
{
    _scannedDarks = darks;

    populateDarkFiltersTreeView(_scannedDarks);

    filterDarks();
}

void DataStore::on_CommandStatusChange(AbstractCommand* command)
{
    // warning, commands are inserted at model top
    updateCommandModelRow(_commandListModel->rowCount() - command->getSerial() -1, command);
}

void DataStore::on_CommandCreated(AbstractCommand *command)
{
    _commandListModel->insertRows(0,1);
    _commandListModel->setData(_commandListModel->index(0,
                                                        1,
                                                        QModelIndex()),
                               QTime::currentTime().toString("hh:mm:ss"));
    _commandListModel->setData(_commandListModel->index(0,
                                                        0,
                                                        QModelIndex()),
                               command->getSerial()+1); // commands are numbered from 0

    updateCommandModelRow(0, command);
}

void DataStore::on_newDarkSources(QStringList paths)
{
    _darkSources = paths;

    QSettings().setValue(Globals::SETTINGKEY_DARK_SOURCES, _darkSources);
}

void DataStore::updateCommandModelRow(int row, AbstractCommand *command)
{

    static QColor redBackground(255, 200, 200);
    static QColor yellowBackground(255, 220, 150);
    static QColor greenBackground(200, 255, 200);

    if ( AbstractCommand::COMPLETE == command->getStatus() ) {

        QColor statusBackground;

        if ( command->hasErrors() ) {

            statusBackground = redBackground;

        } else {

            if ( command->hasWarning() ) {

                statusBackground = yellowBackground;

            } else {

                statusBackground = greenBackground;
            }
        }

        _commandListModel->setData(_commandListModel->index(row,2,QModelIndex()),
                                   statusBackground,
                                   Qt::BackgroundRole);
    }

    _commandListModel->setData(_commandListModel->index(row,2,QModelIndex()),
                               command->getStatusString());

    _commandListModel->setData(_commandListModel->index(row,3,QModelIndex()),
                               command->getDescription());

    _commandListModel->setData(_commandListModel->index(row,4,QModelIndex()),
                               command->getProgessMessage());

    _commandListModel->setData(_commandListModel->index(row,5,QModelIndex()),
                               command->getReportMessages().join('\n'));




}

void DataStore::populateDarkFiltersTreeView(QList<ImageInfo> imageInfos)
{
    /*
     * browse all images and break them down according to this hierarchy
     *
     * - camera serial
     *   - ISO
     *     - Exposure
     */

    QMap< QString, ImageInfo > serialMap;

    foreach (ImageInfo info, imageInfos) {

        serialMap.insertMulti(info.getCameraSerial(), info);
    }

    foreach (QString serial , serialMap.uniqueKeys()) {

        QStandardItem* currentSerialItem = new QStandardItem(tr("Camera N°").append(serial));

        _darkTreeModel->invisibleRootItem()->appendRow(currentSerialItem);
        _darkTreeModel->setData(_darkTreeModel->indexFromItem(currentSerialItem), serial, Qt::UserRole);
        _darkTreeModel->setData(_darkTreeModel->indexFromItem(currentSerialItem),
                                tr("%1 Darks").arg(serialMap.values(serial).count()),
                                Qt::ToolTipRole);

        QMap <int, ImageInfo > isoMap;

        foreach (ImageInfo info, serialMap.values(serial)) {

            isoMap.insertMulti(info.getIso(), info);
        }

        foreach (int iso, isoMap.uniqueKeys()) {

            QStandardItem* currentIsoItem = new QStandardItem(QString::number(iso).append(" ISO"));

            currentSerialItem->appendRow(currentIsoItem);
            _darkTreeModel->setData(_darkTreeModel->indexFromItem(currentIsoItem),
                                    QString(serial).append('|').append(QString::number(iso)),
                                    Qt::UserRole);
            _darkTreeModel->setData(_darkTreeModel->indexFromItem(currentIsoItem),
                                    tr("%1 Darks").arg(isoMap.values(iso).count()),
                                    Qt::ToolTipRole);

            QMap<int, ImageInfo> expoMap;

            foreach (ImageInfo info, isoMap.values(iso)) {

                expoMap.insertMulti(info.getExposure(), info);
            }

            foreach (int expo, expoMap.uniqueKeys()) {

                QStandardItem* currentExpoItem = new QStandardItem(QString::number(expo).append("\""));

                currentIsoItem->appendRow(currentExpoItem);
                _darkTreeModel->setData(
                            _darkTreeModel->indexFromItem(currentExpoItem),
                            QString(serial).append('|')
                            .append(QString::number(iso)).append('|')
                            .append(QString::number(expo)),
                            Qt::UserRole);
                _darkTreeModel->setData(
                            _darkTreeModel->indexFromItem(currentExpoItem),
                            tr("%1 Darks").arg(expoMap.values(expo).count()),
                            Qt::ToolTipRole);
            }
        }
    }
}

void DataStore::on_newDarkScanStarted()
{
    _darkListModel->setRowCount(0);
    _darkTreeModel->setRowCount(0);
    _S_DarkDisplayFilter = "";
}


void DataStore::setRememberWindowGeometry(bool remember)
{
    _rememberWindowGeometry = remember;
    QSettings().setValue(Globals::SETTINGKEY_WINDOW_GEOMETRY_REMEMBER, _rememberWindowGeometry);
}

void DataStore::setScanDarkOnStartup(bool scan)
{
    _scanDarksOnStartup = scan;
    QSettings().setValue(Globals::SETTINGKEY_SCANDARKS_ON_STARTUP, _scanDarksOnStartup);
}

void DataStore::filterDarks()
{
    _filteredDarks = QtConcurrent::blockingFiltered(_scannedDarks,
                                                    &DataStore::filterDark);
    populateDarkListModel(_filteredDarks);
}

void DataStore::setDarkDisplayFilter(const QString filter)
{
    _S_DarkDisplayFilter = filter;

    filterDarks();
}
