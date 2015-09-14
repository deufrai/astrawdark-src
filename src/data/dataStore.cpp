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
    _commandListModel->setColumnCount(6);
    _commandListModel->setHorizontalHeaderLabels(QStringList()
                                                 << tr("N°")
                                                 << tr("Time")
                                                 << tr("Status")
                                                 << tr("Command")
                                                 << tr("Progress")
                                                 << tr("Report"));

    _darkListModel->setColumnCount(7);
    _darkListModel->setHorizontalHeaderLabels(QStringList()
                                              << tr("Path")
                                              << tr("Make")
                                              << tr("Model")
                                              << tr("Exposure\n(sec.)")
                                              << tr("ISO")
                                              << tr("Date")
                                              << tr ("Temperature\n(°C)"));

    _darkTreeModel->setColumnCount(2);
    _darkTreeModel->setHorizontalHeaderLabels(QStringList()
                                              << tr("Shot settings")
                                              << tr("Image count"));

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

void DataStore::breakDownImageInfos(QList<ImageInfo> imageInfos)
{
    /*
     * browse all images and break them down according to this hierarchy
     *
     * - camera serial
     *   - ISO
     *     - Exposure
     */
    QStandardItem *rootNode = _darkTreeModel->invisibleRootItem();

    QMap< QString, ImageInfo > serialMap;

    foreach (ImageInfo info, imageInfos) {

        serialMap.insertMulti(info.getCameraSerial(), info);
    }

    foreach (QString serial , serialMap.uniqueKeys()) {

        QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
        newRowItems->append(new QStandardItem(serial));
        newRowItems->append(new QStandardItem(QString::number(serialMap.values(serial).count())));

        rootNode->appendRow(*newRowItems);

        QMap <QString, ImageInfo > isoMap;

        foreach (ImageInfo info, serialMap.values(serial)) {

            QString formattedIso;
            formattedIso.sprintf("%06d", (info.getIso()));
            isoMap.insertMulti(serial + '|' + formattedIso, info);
        }

        foreach (QString serialIso, isoMap.uniqueKeys()) {

            QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
            newRowItems->append(new QStandardItem(serialIso.split('|').at(1)));
            newRowItems->append(new QStandardItem(QString::number(isoMap.values(serialIso).count())));

            _darkTreeModel->findItems(serialIso.split('|').at(0)).at(0)->appendRow(*newRowItems);

            QMap<QString, ImageInfo> expoMap;

            foreach (ImageInfo info, isoMap.values(serialIso)) {

                QString formattedExposure;
                formattedExposure.sprintf("%04d", (info.getExposure()));

                expoMap.insertMulti(serialIso + '|' + formattedExposure, info);
            }

            foreach (QString serialIsoExpo, expoMap.uniqueKeys()) {

                QList<QStandardItem*> *newRowItems = new QList<QStandardItem*>();
                newRowItems->append(new QStandardItem(serialIsoExpo.split('|').at(2)));
                newRowItems->append(new QStandardItem(QString::number(expoMap.values(serialIsoExpo).count())));

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

    QModelIndex nextSerialIndex = _darkTreeModel->index(0,0);

    while ( nextSerialIndex.isValid() ) {

        _darkTreeModel->setData(nextSerialIndex,
                                "Camera #" + _darkTreeModel->data(nextSerialIndex).toString());

        QModelIndex nextIsoIndex = nextSerialIndex.child(0, 0);

        while ( nextIsoIndex.isValid() ) {

            _darkTreeModel->setData(nextIsoIndex,
                                    _darkTreeModel->data(nextIsoIndex).toString().append(tr(" ISO")).remove(QRegExp("^0*")));

            QModelIndex nextExpoIndex = nextIsoIndex.child(0,0);

            while ( nextExpoIndex.isValid() ) {

                _darkTreeModel->setData(nextExpoIndex,
                                        _darkTreeModel->data( nextExpoIndex ).toString().append(tr(" sec")).remove(QRegExp("^0*")));

                nextExpoIndex = nextExpoIndex.sibling(nextExpoIndex.row() + 1, 0);
            }

            nextIsoIndex = nextIsoIndex.sibling(nextIsoIndex.row() + 1, 0);
        }

        nextSerialIndex = _darkTreeModel->index(nextSerialIndex.row() + 1, 0);

    }
}

void DataStore::on_newDarkScanStarted()
{
    _darkListModel->setRowCount(0);
    _darkTreeModel->setRowCount(0);
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
