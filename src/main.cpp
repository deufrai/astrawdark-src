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

#include "gui/mainWindow.h"
#include "commands/commandManager.h"
#include "data/imageInfo.h"
#include "data/dataStore.h"
#include "globals.h"

#include <QApplication>
#include <QSettings>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("wardsback");
    QCoreApplication::setOrganizationDomain("wardsback.org");
    QCoreApplication::setApplicationName("AstRawDark");
    QSettings settings;

    qRegisterMetaType< QList<ImageInfo> >("QList<ImageInfo>");

    CommandManager* commandManager = new CommandManager();
    MainWindow w(commandManager);

    if ( settings.contains(Globals::SETTINGKEY_WINDOW_GEOMETRY) &&
         DataStore::getInstance()->getRememberWindowGeometry() ) {

        w.setGeometry(settings.value(Globals::SETTINGKEY_WINDOW_GEOMETRY).toRect());
    }


#ifndef QT_NO_DEBUG
    qDebug() << "Showing main window";
#endif
    w.show();

    int nRet = a.exec();

    delete commandManager;

    if ( DataStore::getInstance()->getRememberWindowGeometry() ) {

        settings.setValue(Globals::SETTINGKEY_WINDOW_GEOMETRY, w.geometry());
    }

    settings.sync();

    return nRet;
}
