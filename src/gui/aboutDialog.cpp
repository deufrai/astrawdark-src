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

#include "aboutDialog.h"
#include "ui_aboutDialog.h"
#include "globals.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString aboutString;

    aboutString.append( "<center>" )
            .append( "<h2>" )
            .append( "AstRawDark" )
            .append( "</h2>" )
            .append( "<p>" )
            .append( tr( "Version" ) )
            .append( " : " )
            .append( Globals::VERSION_STRING )
            .append( "</p>" )
            .append( "<p>" )
            .append( tr( "Author" ) )
            .append(
            " : Fr&eacute;d&eacute;ric CORNU <a href=\"mailto:fcornu@wardsback.org\">fcornu@wardsback.org</a>" )
            .append( "</p>" )
            .append( "<p>" )
            .append( tr( "License" ) )
            .append( " : <a href=\"http://www.gnu.org/licenses/gpl-3.0.txt\">General Public License v3</a>" )
            .append( "</p>" )
            .append( "</center>" );

    ui->aboutLabel->setText( aboutString );

    QLabel* lblCreditExiv2 = new QLabel(this);

    lblCreditExiv2->setText(QString("<a href=\"http://www.exiv2.org/\">Exiv2 library</a> : ")
                            .append(tr("EXIF metadata handling")));

    lblCreditExiv2->setOpenExternalLinks(true);
    ui->grpCredits->layout()->addWidget(lblCreditExiv2);

    QLabel* lblCreditsQCustomPlot = new QLabel(this);

    lblCreditsQCustomPlot->setText(QString("<a href=\"http://www.qcustomplot.com/\">QCustomPlot Widget</a> : ")
                                   .append(tr("Data plotting")));

    lblCreditsQCustomPlot->setOpenExternalLinks(true);
    ui->grpCredits->layout()->addWidget(lblCreditsQCustomPlot);

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::changeEvent(QEvent *e)
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
