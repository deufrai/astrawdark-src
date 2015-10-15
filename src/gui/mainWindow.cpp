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
#include "globals.h"
#include "data/dataStore.h"
#include "aboutDialog.h"
#include "commands/signalDispatcher.h"
#include "commandReportDisplay.h"

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      LBL_DARKCOUNT_BASETEXT(tr("Displayed darks count")),
      LBL_LIGHTSCOUNT_BASETEXT(tr("Displayed lights count : %1")),
      ui(new Ui::MainWindow),
      _dataStore(DataStore::getInstance())

{
    ui->setupUi(this);

    _plotManager = new PlotManager(ui->pltDarkTempPlot,
                                   ui->pltDarkTempDistriPlot,
                                   ui->pltLightsTempPlot,
                                   ui->pltLightsTempDistriPlot,
                                   this);

    setWindowTitle(Globals::APPLICATION_NAME);

    ui->tblDarkView->setModel(_dataStore->getDarkModel());
    QHeaderView* darkHv = ui->tblDarkView->horizontalHeader();
    darkHv->setSectionResizeMode(0, QHeaderView::Stretch);
    darkHv->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    darkHv->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    QHeaderView* darkVv = ui->tblDarkView->verticalHeader();
    darkVv->hide();

    ui->tblLightsList->setModel(_dataStore->getLightsModel());
    QHeaderView* lightHv = ui->tblLightsList->horizontalHeader();
    lightHv->setSectionResizeMode(0, QHeaderView::Stretch);
    lightHv->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    lightHv->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    lightHv->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    lightHv->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    lightHv->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    QHeaderView* lightVv = ui->tblLightsList->verticalHeader();
    lightVv->hide();

    ui->tblCommandView->setModel(_dataStore->getCommandListModel());
    QHeaderView* commandHv = ui->tblCommandView->horizontalHeader();
    commandHv->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    commandHv->setSectionResizeMode(6, QHeaderView::Stretch);
    QHeaderView* commandVv = ui->tblCommandView->verticalHeader();
    commandVv->setSectionResizeMode(QHeaderView::ResizeToContents);
    commandVv->hide();

    ui->treeDarkView->setModel(_dataStore->getDarkTreeModel());
    QHeaderView* darkTreeHv = ui->treeDarkView->header();
    darkTreeHv->setSectionResizeMode(0, QHeaderView::Stretch);
    darkTreeHv->hide();

    ui->btnRescanDarks->setEnabled( !_dataStore->getDarkSources().empty() );

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

    connect(this,
            &MainWindow::scanLightsFolder,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_createLightsScanCommand);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::darkListModelChanged,
            this,
            &MainWindow::on_darkListModelChanged);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::lightsScanStarted,
            this,
            &MainWindow::on_lightsScanStart);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::lightsScanDone,
            this,
            &MainWindow::on_lightsScanDone);

    connect(this,
            &MainWindow::checkLights,
            SignalDispatcher::getInstance(),
            &SignalDispatcher::on_createLightsCheckCommand);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::consistencyResult,
            this,
            &MainWindow::on_consistencyResult);

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::commandCreated,
            this,
            &MainWindow::on_commandCreated);

    _commandManager = new CommandManager(this);

    ui->tabDarkDetailsWidget->setCurrentIndex(0);
    ui->tabMainWidget->setCurrentIndex(0);
    ui->tabLightsDetailsWidget->setCurrentIndex(0);

    if ( 0 == _dataStore->getDarkSources().count() ) {

#ifndef QT_NO_DEBUG
        qDebug() << "No dark sources set yet";
#endif
        QMessageBox messageBox(QMessageBox::Question,
                               tr("AstRawDark : No dark sources set"),
                               QString("<h3>")
                               .append(tr("Your darks library sources are not configured"))
                               .append("</h3>")
                               .append(tr("Would you like to review your darks library settings ?")),
                               QMessageBox::Yes | QMessageBox::No,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("Yes"));
        messageBox.setButtonText(QMessageBox::No, tr("No"));

        if ( QMessageBox::Yes == messageBox.exec() ) {

            PrefDialog(this).exec();
        }
    }

    updateLightsContentCount();
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
    int darkLibrarySize = _dataStore->getDarkLibrarySize();
    int darkModelSize = _dataStore->getDarkModel()->rowCount();

    if ( darkLibrarySize != darkModelSize ) {

        darkContentTabText.append(tr(" (Filtered)"));
    }

    darkContentTabText.append(tr(" : %1 / %2").arg(darkModelSize).arg(darkLibrarySize));

    ui->lblDarkCount->setText(darkContentTabText);
}

void MainWindow::updateLightsContentCount()
{
    ui->lblLightsCount->setText(LBL_LIGHTSCOUNT_BASETEXT.arg(_dataStore->getLightsCount()));
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

    _dataStore->setDarkDisplayFilter(targetNode.data(Qt::UserRole).toString());
}

void MainWindow::on_btnDarkFilterClear_clicked()
{
    ui->treeDarkView->clearSelection();
    ui->btnDarkFilterClear->setDisabled(true);
    _dataStore->setDarkDisplayFilter("");
}

void MainWindow::on_darkListModelChanged()
{
    updateDarkContentCount();
    ui->tblDarkView->scrollToTop();
}

void MainWindow::on_actionRescanDarksLibrary_triggered()
{
    emit scanDarkLibrary();
}

void MainWindow::on_btnChooseLightsFolder_clicked()
{
    QString startFolder = ui->lineCurrentLightsFolderPath->text();

    if ( startFolder.isEmpty() || ! QDir(startFolder).exists() ) {

        startFolder = QDir::homePath();
    }

    QString lightsFolder = QFileDialog::getExistingDirectory(this,
                                                             tr("Please select lights folder"),
                                                             startFolder,
                                                             QFileDialog::ShowDirsOnly);

    if ( ! lightsFolder.isEmpty() && lightsFolder != startFolder ) {

        ui->lineCurrentLightsFolderPath->setText(lightsFolder);
        _dataStore->setLightsFolder(lightsFolder);

        emit scanLightsFolder();
    }

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
    ui->actionRescanDarksLibrary->setDisabled(true);
}

void MainWindow::on_darkScanDone()
{

    ui->btnRescanDarks->setEnabled(true);
    ui->actionRescanDarksLibrary->setEnabled(true);
}

void MainWindow::on_lightsScanStart()
{
    ui->lblLightsCount->setText(LBL_LIGHTSCOUNT_BASETEXT.arg("Scan in progress..."));
    ui->btnChooseLightsFolder->setDisabled(true);
    ui->btnRescanLights->setDisabled(true);
}

void MainWindow::on_lightsScanDone()
{
    ui->btnChooseLightsFolder->setEnabled(true);
    ui->btnRescanLights->setEnabled(true);

    updateLightsContentCount();
    ui->tblLightsList->scrollToTop();

    if ( _dataStore->getLightsCount() > 0 ) {

        emit checkLights();
    }

}

void MainWindow::on_consistencyResult(bool consistent)
{
    if ( ! consistent ) {

        QMessageBox::warning(this,
                             tr("Lights are not consistent"),
                             tr("These lights have not been shot with the same camera"
                                " or have different ISO settings and exposure times.")
                             .append("\n\n")
                             .append(tr("Temperature matching cannot be performed")));
    }
}

void MainWindow::createProgressBarForCommand(AbstractCommand *command)
{
    QProgressBar* progBar = new QProgressBar(this);
    progBar->setMaximum(0);
    progBar->setMinimumWidth(Globals::COMMAND_LOG_PROGRESS_WIDTH);

    connect(command,
            &AbstractCommand::progress,
            progBar,
            &QProgressBar::setValue, Qt::BlockingQueuedConnection);

    connect(command,
            &AbstractCommand::progressMax,
            progBar,
            &QProgressBar::setMaximum, Qt::BlockingQueuedConnection);

    QAbstractItemModel* model = ui->tblCommandView->model();
    ui->tblCommandView->setIndexWidget(model->index(model->rowCount() - command->getSerial() -1, 4, QModelIndex()), progBar);
}

void MainWindow::on_commandCreated(AbstractCommand *command)
{
    ui->tblCommandView->scrollToTop();

    createProgressBarForCommand(command);
}

void MainWindow::on_btnRescanLights_clicked()
{
	emit scanLightsFolder();
}
