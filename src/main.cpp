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

#include "gui/mainwindow.h"
#include <QApplication>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

#ifndef QT_NO_DEBUG
    qDebug() << "Showing main window";
#endif

    w.show();

    return a.exec();
}
