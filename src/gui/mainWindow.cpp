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
#include "../data/dataStore.h"
#include "aboutDialog.h"
#include "../commands/signalDispatcher.h"
#include "commandReportDisplay.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(CommandManager *manager, QWidget *parent)
    : QMainWindow(parent),
      _commandManager(manager),
      LBL_DARKCOUNT_BASETEXT(tr("Displayed darks count")),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _plotManager = new PlotManager(ui->pltDarkTempPlot, ui->pltDarkTempDistriPlot, this);

    setWindowTitle(Globals::APPLICATION_NAME);

    ui->tblDarkView->setModel(DataStore::getInstance()->getDarkModel());
    QHeaderView* darkHv = ui->tblDarkView->horizontalHeader();
    darkHv->setSectionResizeMode(0, QHeaderView::Stretch);
    darkHv->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    QHeaderView* darkVv = ui->tblDarkView->verticalHeader();
    darkVv->hide();

    ui->tblCommandView->setModel(DataStore::getInstance()->getCommandListModel());
    QHeaderView* commandHv = ui->tblCommandView->horizontalHeader();
    commandHv->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(5, QHeaderView::Stretch);
    QHeaderView* commandVv = ui->tblCommandView->verticalHeader();
    commandVv->setSectionResizeMode(QHeaderView::ResizeToContents);
    commandVv->hide();

    ui->treeDarkView->setModel(DataStore::getInstance()->getDarkTreeModel());
    QHeaderView* darkTreeHv = ui->treeDarkView->header();
    darkTreeHv->setSectionResizeMode(0, QHeaderView::Stretch);
    darkTreeHv->hide();

    ui->btnRescanDarks->setEnabled( !DataStore::getInstance()->getDarkSources().empty() );

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkSourcesChanged,
            this,
            &MainWindow::on_darkSourcesChanged);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanStarted,
            this,
            &MainWindow::on_darkScanStart);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkScanDone,
            this,
            &MainWindow::on_darkScanDone);

    connect(this,
            &MainWindow::scanDarkLibrary,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_createDarkScanCommand);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkListModelChanged,
            this,
            &MainWindow::on_darkListModelChanged);

    ui->tabDarkDetailsWidget->setCurrentIndex(0);
    ui->tabMainWidget->setCurrentIndex(0);
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
    emit scanDarkLibrary();
}

void MainWindow::on_actionPrefs_triggered()
{
    PrefDialog(this).exec();
}

void MainWindow::on_actionCommandLog_toggled(bool checked)
{
    ui->dckCommands->setVisible(checked);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog(this).exec();
}

void MainWindow::on_tblCommandView_doubleClicked(const QModelIndex &index)
{
    int commandSerial = index.model()->data(index.model()->index(index.row(), 0)).toInt() -1;

    const AbstractCommand* command = _commandManager->getCommand(commandSerial);

    CommandReportDisplay* d = new CommandReportDisplay(command);
    d->show();
}

void MainWindow::updateDarkContentCount()
{

    QString darkContentTabText = LBL_DARKCOUNT_BASETEXT;
    int darkLibrarySize = DataStore::getInstance()->getDarkLibrarySize();
    int darkModelSize = DataStore::getInstance()->getDarkModel()->rowCount();

    if ( darkLibrarySize != darkModelSize ) {

        darkContentTabText.append(tr(" (Filtered)"));
    }

    darkContentTabText.append(tr(" : %1 / %2").arg(darkModelSize).arg(darkLibrarySize));

    ui->lblDarkCount->setText(darkContentTabText);
}

void MainWindow::on_treeDarkView_clicked(const QModelIndex &index)
{
    ui->btnDarkFilterClear->setEnabled(true);

    QModelIndex targetNode;

    if ( index.column() != 0 ) {

        targetNode = index.sibling(index.row(), 0);

    } else {

        targetNode = index;
    }

    DataStore::getInstance()->setDarkDisplayFilter(targetNode.data(Qt::UserRole).toString());
}

void MainWindow::on_btnDarkFilterClear_clicked()
{
    ui->treeDarkView->clearSelection();
    ui->btnDarkFilterClear->setDisabled(true);
    DataStore::getInstance()->setDarkDisplayFilter("");
}

void MainWindow::on_darkListModelChanged()
{
    updateDarkContentCount();
    ui->tblDarkView->scrollToTop();
}

void MainWindow::on_darkSourcesChanged(const QStringList& sources)
{
    ui->btnRescanDarks->setEnabled( !sources.empty() );
}

void MainWindow::on_darkScanStart()
{
    ui->lblDarkCount->setText(LBL_DARKCOUNT_BASETEXT + tr(" : Scan in progress..."));
    ui->btnRescanDarks->setDisabled(true);
    ui->btnDarkFilterClear->setDisabled(true);
}

void MainWindow::on_darkScanDone()
{

    ui->btnRescanDarks->setEnabled(true);
}
