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
    : QObject(0),
      _darkListModel(new QStandardItemModel()),
      _commandListModel(new QStandardItemModel())
{
    QSettings settings;

    if ( settings.contains(Globals::SETTINGKEY_DARK_SOURCES) ) {

        _darkSources = settings.value(Globals::SETTINGKEY_DARK_SOURCES).toStringList();
    }

    _commandListModel->setColumnCount(3);
    _commandListModel->setHorizontalHeaderLabels(QStringList()
                                                 << tr("Time")
                                                 << tr("Command")
                                                 << tr("Status"));

    _darkListModel->setColumnCount(7);
    QStringList darkListModelHeaderLabels;

    darkListModelHeaderLabels << tr("Path")
                              << tr("Make")
                              << tr("Model")
                              << tr("Exposure")
                              << tr("ISO")
                              << tr("Date")
                              << tr ("Temperature");

    _darkListModel->setHorizontalHeaderLabels(darkListModelHeaderLabels);
}

void DataStore::on_newDarkScanResult(QList<ImageInfo> darks)
{

    _darkListModel->setRowCount(0);
    _darkListModel->setRowCount(darks.count());

    int row = 0;

    foreach (ImageInfo info, darks) {

        _darkListModel->setData(_darkListModel->index(row, 0, QModelIndex()),
                                QString(info.getPath().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 1, QModelIndex()),
                                QString(info.getMake().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 2, QModelIndex()),
                                QString(info.getModel().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 3, QModelIndex()),
                                QString(info.getExposure().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 4, QModelIndex()),
                                QString(info.getIso().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 5, QModelIndex()),
                                QString(info.getDate().c_str()));

        _darkListModel->setData(_darkListModel->index(row, 6, QModelIndex()),
                                QString::number(info.getTemperature()).append("°C"));

        ++row;

    }

    emit darkListUpdated();
}

void DataStore::on_CommandStatusChange(AbstractCommand* command)
{
    updateCommandModel(command->getSerial(), command);
}

void DataStore::on_CommandCreated(AbstractCommand *command)
{
    _commandListModel->setRowCount(_commandListModel->rowCount()+1);
    _commandListModel->setData(_commandListModel->index(_commandListModel->rowCount()-1,0,QModelIndex()),
                               QTime::currentTime().toString("hh:mm:ss"));
    updateCommandModel(_commandListModel->rowCount()-1, command);
    emit commandAdded();

}

void DataStore::on_newDarkSources(QStringList paths)
{
    _darkSources = paths;

    if ( _darkSources.empty() ) {

        _darkListModel->clear();
        emit darkListUpdated();
    }

    QSettings().setValue(Globals::SETTINGKEY_DARK_SOURCES, _darkSources);

    emit darkSourcesChanged(_darkSources);
}

void DataStore::updateCommandModel(int row, AbstractCommand *command)
{
    QString status;
    switch ( command->getStatus() ) {

    case AbstractCommand::PENDING:
        status = tr("Scheduled");
        break;

    case AbstractCommand::RUNNING:
        status = tr("Running");
        break;

    case AbstractCommand::COMPLETE:
        status = tr("Finished (%1 ms)").arg(command->getElapsed());
        break;

    default:
        status = tr("Undefined");
        break;
    }

    _commandListModel->setData(_commandListModel->index(row,1,QModelIndex()),
                               command->getDescription());

    _commandListModel->setData(_commandListModel->index(row,2,QModelIndex()),
                               QString(status));
}


