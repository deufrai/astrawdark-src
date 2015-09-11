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


#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

DataStore* DataStore::_instance = 0;

DataStore* DataStore::getInstance() {

    if ( 0 == _instance ) _instance = new DataStore();

    return _instance;
}

DataStore::DataStore()
    : _darkListModel(new QStandardItemModel()),
      _commandListModel(new QStandardItemModel()),
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
    _commandListModel->setColumnCount(4);
    _commandListModel->setHorizontalHeaderLabels(QStringList()
                                                 << tr("Time")
                                                 << tr("Status")
                                                 << tr("Command")
                                                 << tr("Details"));

    _darkListModel->setColumnCount(7);
    QStringList darkListModelHeaderLabels;

    darkListModelHeaderLabels << tr("Path")
                              << tr("Make")
                              << tr("Model")
                              << tr("Exposure\n(sec.)")
                              << tr("ISO")
                              << tr("Date")
                              << tr ("Temperature\n(°C)");

    _darkListModel->setHorizontalHeaderLabels(darkListModelHeaderLabels);

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

}

void DataStore::on_newDarkScanResult(QList<ImageInfo> darks)
{

    _darkListModel->setRowCount(0);
    _darkListModel->setRowCount(darks.count());

    int row = 0;

    foreach (ImageInfo info, darks) {

        _darkListModel->setData(_darkListModel->index(row, 0, QModelIndex()),
                                info.getPath());

        _darkListModel->setData(_darkListModel->index(row, 1, QModelIndex()),
                                info.getMake());

        _darkListModel->setData(_darkListModel->index(row, 2, QModelIndex()),
                                info.getModel());

        _darkListModel->setData(_darkListModel->index(row, 3, QModelIndex()),
                                info.getExposure());

        _darkListModel->setData(_darkListModel->index(row, 4, QModelIndex()),
                                info.getIso());

        _darkListModel->setData(_darkListModel->index(row, 5, QModelIndex()),
                                info.getDate());

        _darkListModel->setData(_darkListModel->index(row, 6, QModelIndex()),
                                QString::number(info.getTemperature()));

        ++row;

    }

    _darkListModel->sort(0);
}

void DataStore::on_CommandStatusChange(AbstractCommand* command)
{
    updateCommandModel(command->getSerial(), command);
}

void DataStore::on_CommandCreated(AbstractCommand *command)
{
    _commandListModel->setRowCount(_commandListModel->rowCount()+1);
    _commandListModel->setData(_commandListModel->index(_commandListModel->rowCount()-1,
                                                        0,
                                                        QModelIndex()),
                               QTime::currentTime().toString("hh:mm:ss"));

    updateCommandModel(_commandListModel->rowCount()-1, command);
}

void DataStore::on_newDarkSources(QStringList paths)
{
    _darkSources = paths;

    QSettings().setValue(Globals::SETTINGKEY_DARK_SOURCES, _darkSources);
}

void DataStore::updateCommandModel(int row, AbstractCommand *command)
{
    QString status;
    switch ( command->getStatus() ) {

    case AbstractCommand::SCHEDULED:
        status = tr("Scheduled");
        break;

    case AbstractCommand::RUNNING:
        status = tr("Running");
        break;

    case AbstractCommand::COMPLETE:
        status = tr("Finished (%1 ms)").arg(command->getElapsed());

        if ( command->hasErrors() ) {

            for (int col=0; col<_commandListModel->columnCount(); col++) {

                _commandListModel->setData(_commandListModel->index(row,col,QModelIndex()),
                                           QColor(255, 200, 200),
                                           Qt::BackgroundRole);
            }

        } else {

            for (int col=0; col<_commandListModel->columnCount(); col++) {

                _commandListModel->setData(_commandListModel->index(row,col,QModelIndex()),
                                           QColor(200, 255, 200),
                                           Qt::BackgroundRole);
            }
        }
        break;

    default:
        status = tr("Undefined");
        break;
    }

    QString description = command->getDescription();

    _commandListModel->setData(_commandListModel->index(row,2,QModelIndex()),
                               description);

    if ( command->hasErrors() ) {

        _commandListModel->setData(_commandListModel->index(row,3,QModelIndex()),
                                   command->getProgessMessage()
                                   .append(" - ")
                                   .append(tr("*Error* "))
                                   .append(command->getErrorMessage()));

    } else {

        _commandListModel->setData(_commandListModel->index(row,3,QModelIndex()),
                                   command->getProgessMessage());

    }

    _commandListModel->setData(_commandListModel->index(row,1,QModelIndex()),
                               status);


}

void DataStore::on_newDarkScanStarted()
{
    _darkListModel->setRowCount(0);
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
