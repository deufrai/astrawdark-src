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

/**
 * @mainpage Welcome to AstRawDark
 *
 * @section intro_sec Introduction
 *
 * AstRawDark helps you manage your astrophotography dark frames library and
 * generate quality master darks by selecting dark frames with temperature matching
 * the lights you have to pre-process
 *
 * In here you'll find AstRawDark's C++ source code documentation. It will help you get a grip
 * on how the various components work & interact with eachother.
 *
 * @section dowload_sec Downloads
 *
 * binaries for the main platforms (GNU/linux .deb, OSX & Windows) are available here :
 * http://wardsback.org/pub/astrawdark/dls
 *
 * @section info_sec Developpement tools
 *
 * Source code browser :
 * http://code.wardsback.org/projects/ARD/repos/astrawdark-src/browse
 *
 * Developpment tasks & issues :
 * http://requests.wardsback.org/projects/ARD/summary/statistics
 *
 * Have fun
 *
 */

#include "gui/mainWindow.h"
#include "data/imageInfo.h"
#include "data/dataStore.h"
#include "globals.h"
#include "helpers/localeHelper.h"

#include <QApplication>
#include <QSettings>
#include <QTranslator>

#ifdef Q_OS_MAC
#include <QLibraryInfo>
#endif

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

void installTranslator( QApplication& app, QString& filePrefix, QString& folderPath, QString locale );

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("wardsback");
    QCoreApplication::setOrganizationDomain("wardsback.org");
    QCoreApplication::setApplicationName("AstRawDark");
    QSettings settings;

    qRegisterMetaType< QList<ImageInfo> >("QList<ImageInfo>");

    QString appTransfilePrefix = "astrawdark_";
    QString appTransFolderPath = ":/i18n";
    installTranslator( a, appTransfilePrefix, appTransFolderPath, LocaleHelper::getLocale() );

#ifdef Q_OS_MAC

    /*
     * on Mac :
     *
     * - some menu entries are merged into the "application menu" and their translations are
     *   provided by a Qt specific translation file
     *
     * - no icons are shown next to menu items
     */

    // install translator for Qt itself
    QString qtTransfilePrefix= "qt_";
    QString qtTransFolderPath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    installTranslator(a, qtTransfilePrefix, qtTransFolderPath, LocaleHelper::getLocale());

    // Don't show icons for menu items on Mac
    a.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    MainWindow w;

    if ( DataStore::getInstance()->getRememberWindowGeometry() ) {

        if ( settings.value(Globals::SETTINGKEY_WINDOW_MAXIMIZED, false).toBool() ) {

            w.setWindowState( w.windowState() | Qt::WindowMaximized );

        } else {

            if ( settings.contains(Globals::SETTINGKEY_WINDOW_GEOMETRY) ) {

                w.setGeometry(settings.value(Globals::SETTINGKEY_WINDOW_GEOMETRY).toRect());
            }
        }

    }


#ifndef QT_NO_DEBUG
    qDebug() << "Showing main window";
#endif
    w.show();

    int nRet = a.exec();

    if ( DataStore::getInstance()->getRememberWindowGeometry() ) {

        settings.setValue(Globals::SETTINGKEY_WINDOW_GEOMETRY, w.geometry());
        settings.setValue(Globals::SETTINGKEY_WINDOW_MAXIMIZED, Qt::WindowMaximized == w.windowState());
    }

    settings.sync();

    return nRet;
}

void installTranslator( QApplication& app, QString& filePrefix, QString& folderPath, QString locale ) {

    QTranslator* pTranslator = new QTranslator();
    bool isTransLoaded = pTranslator->load( filePrefix + locale, folderPath );

    if( isTransLoaded ) {

#ifndef QT_NO_DEBUG
        qDebug() << "Translation file loaded successfully :" << filePrefix + locale;
#endif

        app.installTranslator( pTranslator );

    } else {

        qWarning( "Failed to load translation file : %s%s", filePrefix.toStdString().c_str(),
                locale.toStdString().c_str() );
    }

}
