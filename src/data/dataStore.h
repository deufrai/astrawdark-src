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

#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
#include <QStandardItemModel>
#include <QSettings>

#include "imageInfo.h"

/**
 * @brief Stores all application data.
 */
class DataStore : public QObject
{
    Q_OBJECT

private:
    DataStore();
public:
    static DataStore* _instance;
    static DataStore* getInstance();

    QStandardItemModel* getDarkModel() const {return _darkListModel;}
    const QStringList& getDarkSources() const {return _darkSources;}

private:
    QStandardItemModel* _darkListModel;
    QStringList         _darkSources;

signals:
    void darkListUpdated();
    void darkSourcesChanged(const QStringList& sources);

public slots:
    void on_newDarkScanResult(QList<ImageInfo> darks);
    void on_newDarkSources(QStringList paths);
};

#endif // DATASTORE_H
