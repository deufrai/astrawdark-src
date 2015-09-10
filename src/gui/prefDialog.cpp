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

#include "prefDialog.h"
#include "ui_prefDialog.h"
#include "../data/dataStore.h"
#include "../globals.h"

#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
    ui->setupUi(this);

    ui->lstDarkFolders->addItems(DataStore::getInstance()->getDarkSources());
    ui->lstDarkFolders->clearSelection();

    ui->chkRemeberWindowGeometry->setChecked(DataStore::getInstance()->getRememberWindowGeometry());
    ui->chkDarkScanOnStartup->setChecked(DataStore::getInstance()->getScanDarkOnStartup());
}

PrefDialog::~PrefDialog()
{
    delete ui;
}

void PrefDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PrefDialog::on_btnAddDarkFolder_clicked()
{
    ui->lstDarkFolders->clearSelection();

    QString basefolder = QFileDialog::getExistingDirectory(this,
                                                           tr("Please select dark base folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly);

    if ( ! basefolder.isEmpty() ) {

        // check if folder is a child, parent or duplicate of any other source folder
        QDir candidate(basefolder);
        bool canUseThisFolder = true;

        for (int i=0; i<ui->lstDarkFolders->count(); i++) {

            QDir test(ui->lstDarkFolders->item(i)->text());

            if ( test.exists() ) {

                if ( test.canonicalPath().startsWith(candidate.canonicalPath()) ||
                     candidate.canonicalPath().startsWith(test.canonicalPath()) ||
                     test.absolutePath().startsWith(candidate.absolutePath())   ||
                     candidate.absolutePath().startsWith(test.absolutePath()) ) {

                    QString warningMsg(tr("Folder '")
                                       .append(basefolder)
                                       .append(tr("' cannot be used as a dark source.\n"))
                                       .append(tr("It is either a child, a parent or a duplicate"))
                                       .append(tr(" of one of your existing sources.")));

                    QMessageBox::critical(this,
                                          tr("Cannot add this folder as dark source"),
                                          warningMsg);

                    canUseThisFolder = false;
                }
            }
        }

        if ( canUseThisFolder ) {

            ui->lstDarkFolders->addItem(basefolder);
        }
    }
}

void PrefDialog::on_lstDarkFolders_itemSelectionChanged()
{
    /*
     * if one item is selected in the dark sources list
     * we activate the 'remove' button
     */

    ui->btnRemoveDarkFolder->setEnabled( ! ui->lstDarkFolders->selectedItems().empty() );
}

void PrefDialog::on_btnRemoveDarkFolder_clicked()
{
    if ( ! ui->lstDarkFolders->selectedItems().empty() ) {

        ui->lstDarkFolders->takeItem(ui->lstDarkFolders->currentRow());
    }
}

void PrefDialog::accept()
{
    _darkSources.clear();

    for ( int i=0; i<ui->lstDarkFolders->count(); i++ ) {

        _darkSources << ui->lstDarkFolders->item(i)->text();
    }

#ifndef QT_NO_DEBUG
    qDebug() << "Sources : " << _darkSources;
#endif

    emit newDarkSources(_darkSources);

    DataStore::getInstance()->setRememberWindowGeometry(ui->chkRemeberWindowGeometry->isChecked());
    DataStore::getInstance()->setScanDarkOnStartup(ui->chkDarkScanOnStartup->isChecked());

    QDialog::accept();
}
