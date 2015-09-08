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

#include "mainWindow.h"
#include "prefDialog.h"
#include "ui_mainWindow.h"
#include "../globals.h"
#include "../commands/abstractCommand.h"
#include "../commands/commandFactory.h"
#include "../data/dataStore.h"

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

    setWindowTitle(Globals::APPLICATION_NAME);

    ui->tblDarkView->setModel(DataStore::getInstance()->getDarkModel());
    ui->tblDarkView->horizontalHeader()->setStretchLastSection(true);
    ui->tblDarkView->resizeColumnsToContents();

    ui->btnRescanDarks->setEnabled( !DataStore::getInstance()->getDarkSources().empty() );

    connect(DataStore::getInstance(), &DataStore::darkListUpdated, this, &MainWindow::on_darkListUpdated);
    connect(DataStore::getInstance(), &DataStore::darkSourcesChanged, this, &MainWindow::on_darkSourcesChanged);

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

void MainWindow::on_btnRescanDarks_clicked()
{
    AbstractCommand* command = CommandFactory::createScanDarkSourceCommand(DataStore::getInstance()->getDarkSources());

#ifndef QT_NO_DEBUG
    qDebug() << "Executing command:" << command->getDescription();
#endif

    command->execute();
    delete command;
}

void MainWindow::on_darkListUpdated()
{
    ui->tblDarkView->resizeColumnsToContents();
    ui->lblDarkCount->setText(QString("Total frame count : %1").arg(DataStore::getInstance()->getDarkModel()->rowCount()));
}

void MainWindow::on_actionPrefs_triggered()
{
    PrefDialog dlg;

    connect(&dlg, &PrefDialog::newDarkSources, DataStore::getInstance(), &DataStore::on_newDarkSources);

    dlg.exec();
}

void MainWindow::on_darkSourcesChanged(const QStringList& sources)
{
    ui->btnRescanDarks->setEnabled( !sources.empty() );
}
