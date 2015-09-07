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

#include "datastore.h"
#include "imageinfo.h"

DataStore* DataStore::_instance = 0;

DataStore* DataStore::getInstance() {

    if ( 0 == _instance ) _instance = new DataStore();

    return _instance;
}

DataStore::DataStore() : QObject(0), _darkListModel(new QStandardItemModel())
{

}

void DataStore::registerDarks(QList<ImageInfo *> darks)
{

    _darkListModel->clear();
    _darkListModel->setRowCount(darks.count());
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

    int row = 0;

    foreach (ImageInfo* info, darks) {

        _darkListModel->setData(_darkListModel->index(row, 0, QModelIndex()),
                                QString(info->getPath().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 1, QModelIndex()),
                                QString(info->getMake().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 2, QModelIndex()),
                                QString(info->getModel().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 3, QModelIndex()),
                                QString(info->getExposure().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 4, QModelIndex()),
                                QString(info->getIso().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 5, QModelIndex()),
                                QString(info->getDate().c_str()),
                                Qt::DisplayRole);

        _darkListModel->setData(_darkListModel->index(row, 6, QModelIndex()),
                                QString::number(atoi(info->getTemperature().c_str()) - 128).append("°C"),
                                Qt::DisplayRole);

        ++row;

    }
}



