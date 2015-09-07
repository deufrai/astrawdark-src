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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../globals.h"
#include "../commands/abstractcommand.h"
#include "../commands/commandfactory.h"
#include "../data/datastore.h"

#include <QFileDialog>
#include <QDir>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1").arg(Globals::application_name));

    ui->tblDarkView->setModel(DataStore::getInstance()->getDarkModel());
    ui->tblDarkView->horizontalHeader()->setStretchLastSection(true);
    ui->tblDarkView->resizeColumnsToContents();

    connect(DataStore::getInstance(), &DataStore::darkListUpdated, this, &MainWindow::on_darkListUpdated);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionSelectDarkFramesFolder_triggered()
{
#ifndef QT_NO_DEBUG
    qDebug() << "Selecting dark frames folder";
#endif

    QString basefolder = QFileDialog::getExistingDirectory(this,
                                                           tr("Please select dark base folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly);

#ifndef QT_NO_DEBUG
    qDebug() << "Base Folder" << basefolder;
#endif

    AbstractCommand* command = CommandFactory::createScanDarkSourceCommand(basefolder.toStdString().c_str());
    command->execute();
    delete command;
}

void MainWindow::on_darkListUpdated()
{
    ui->tblDarkView->resizeColumnsToContents();
    ui->lblDarkCount->setText(QString("Total frame count : %1").arg(DataStore::getInstance()->getDarkModel()->rowCount()));
}
