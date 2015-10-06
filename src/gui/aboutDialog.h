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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
    class AboutDialog;
}

/**
 *@brief Our About dialog
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Qt parent
     */
    explicit AboutDialog(QWidget *parent = 0);

    /** Desctructor */
    ~AboutDialog();

protected:
    /**
     * Caught a change event
     * @param e the event
     */
    void changeEvent(QEvent *e);

private:
    /** Our UI */
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
