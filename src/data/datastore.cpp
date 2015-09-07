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
    _darkListModel->setColumnCount(1);

    int row = 0;

    foreach (ImageInfo* info, darks) {

        QModelIndex index = _darkListModel->index(row++, 0, QModelIndex());
        _darkListModel->setData(index, QString(info->getPath().c_str()), Qt::DisplayRole);
    }
}



