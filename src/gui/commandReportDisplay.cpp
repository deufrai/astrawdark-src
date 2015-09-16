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

#include "commandReportDisplay.h"
#include "../commands/abstractCommand.h"
#include "ui_commandReportDisplay.h"
#include "../commands/signalDispatcher.h"

#include <QListWidget>

CommandReportDisplay::CommandReportDisplay(const AbstractCommand *command, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CommandReportDisplay),
    _command(command)
{
    ui->setupUi(this);

    ui->lblSerial->setText(QString::number(_command->getSerial() +1));
    ui->lblDescription->setText(_command->getDescription());
    ui->grpReports->setLayout(new QVBoxLayout());

    setWindowTitle(tr("Details for command N°%1").arg(_command->getSerial() +1));

    updateDisplay();

    connect(SignalDispatcher::getInstance(),
            &SignalDispatcher::commandStatusChange,
            this,
            &CommandReportDisplay::on_commandUpdate);
}

CommandReportDisplay::~CommandReportDisplay()
{
    delete ui;
}

void CommandReportDisplay::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void CommandReportDisplay::updateDisplay()
{
    ui->lblStatus->setText(_command->getStatusString());
    ui->lblProgress->setText(_command->getProgessMessage());

    /*
     * If command is complete
     * We add command reports if not already done
     */
    if ( AbstractCommand::COMPLETE == _command->getStatus() && ui->grpReports->layout()->count() == 0 ) {

        for ( int i=0; i<_command->getCommandReport()->getHeaders().count(); i++ ) {

            ui->grpReports->layout()->addWidget(new QLabel(_command->getCommandReport()->getHeaders().at(i), this));

            QStringList currentContent = _command->getCommandReport()->getContents().at(i);

            /*
             * some report sections may only have headers
             */
            if ( currentContent.count() > 0 ) {

                QListWidget* list = new QListWidget(this);
                list->addItems(currentContent);
                ui->grpReports->layout()->addWidget(list);
            }
        }
    }
}

void CommandReportDisplay::on_commandUpdate(AbstractCommand *command)
{
    if ( command == _command ) {

        updateDisplay();
    }
}
