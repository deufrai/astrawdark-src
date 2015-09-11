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
    _scannedDarks = darks;

    breakDownImageInfos(_scannedDarks);

    _darkListModel->setRowCount(0);
    _darkListModel->setRowCount(_scannedDarks.count());

    int row = 0;

    foreach (ImageInfo info, _scannedDarks) {

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
                                info.getTemperature() == ImageInfo::UNDEFINED?
                                    "":
                                    QString::number(info.getTemperature()));

        ++row;

    }

    _darkListModel->sort(0);
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
                                                        0,
                                                        QModelIndex()),
                               QTime::currentTime().toString("hh:mm:ss"));

    updateCommandModelRow(0, command);
}

void DataStore::on_newDarkSources(QStringList paths)
{
    _darkSources = paths;

    QSettings().setValue(Globals::SETTINGKEY_DARK_SOURCES, _darkSources);
}

void DataStore::updateCommandModelRow(int row, AbstractCommand *command)
{
    QString status;
    QColor statusBackground;

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

            statusBackground = QColor(255, 200, 200);

        } else {

            statusBackground = QColor(200, 255, 200);
        }

        _commandListModel->setData(_commandListModel->index(row,1,QModelIndex()),
                                   statusBackground,
                                   Qt::BackgroundRole);
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
                                   command->getErrorMessage());

    } else {

        _commandListModel->setData(_commandListModel->index(row,3,QModelIndex()),
                                   command->getProgessMessage());

    }

    _commandListModel->setData(_commandListModel->index(row,1,QModelIndex()),
                               status);


}

void DataStore::breakDownImageInfos(QList<ImageInfo> imageInfos)
{
    /*
     * browse all images and break them down according to this hierarchy
     *
     * - camera serial
     *   - ISO
     *     - Exposure
     */
    _darkTreeModel->setRowCount(0);
    QStandardItem *rootNode = _darkTreeModel->invisibleRootItem();

    QMap< QString, ImageInfo > serialMap;

    foreach (ImageInfo info, imageInfos) {

        serialMap.insertMulti(info.getCameraSerial(), info);
    }

    foreach (QString serial , serialMap.uniqueKeys()) {

        QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
        newRowItems->append(new QStandardItem(serial));
        newRowItems->append(new QStandardItem(QString::number(serialMap.values(serial).count()).append(tr(" Images"))));

        rootNode->appendRow(*newRowItems);

        QMap <QString, ImageInfo > isoMap;

        foreach (ImageInfo info, serialMap.values(serial)) {

          QString formattedIso;
          formattedIso.sprintf("%06d", (info.getIso().toInt()));
            isoMap.insertMulti(serial + '|' + formattedIso, info);
        }

        foreach (QString serialIso, isoMap.uniqueKeys()) {

            QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
            newRowItems->append(new QStandardItem(serialIso.split('|').at(1)));
            newRowItems->append(new QStandardItem(QString::number(isoMap.values(serialIso).count()).append(tr(" Images"))));

            _darkTreeModel->findItems(serialIso.split('|').at(0)).at(0)->appendRow(*newRowItems);

            QMap<QString, ImageInfo> expoMap;

            foreach (ImageInfo info, isoMap.values(serialIso)) {

                QString formattedExposure;
                formattedExposure.sprintf("%06.2f", (info.getExposure().toDouble()));

                  expoMap.insertMulti(serialIso + '|' + formattedExposure, info);
            }

            foreach (QString serialIsoExpo, expoMap.uniqueKeys()) {

                QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
                newRowItems->append(new QStandardItem(serialIsoExpo.split('|').at(2)));
                newRowItems->append(new QStandardItem(QString::number(expoMap.values(serialIsoExpo).count()).append(tr(" Images"))));

                QList<QStandardItem*> found = _darkTreeModel->findItems(serialIsoExpo.split('|').at(1), Qt::MatchRecursive);

                if ( found.count() > 0 ) {

                    found.at(0)->appendRow(*newRowItems);
                }


#ifndef QT_NO_DEBUG
            qDebug() << QString("SerialIsoExpo %1 : %2 images").arg(serialIsoExpo).arg(expoMap.values(serialIsoExpo).count());
#endif
            }
        }
    }

    _darkTreeModel->setData(_darkTreeModel->index(0,0), tr("Camera N° ").append(_darkTreeModel->index(0,0).data().toString()));
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
