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

#ifndef PREFDIALOG_H
#define PREFDIALOG_H

#include <QDialog>

namespace Ui {
class PrefDialog;
}

/**
 * @brief Dialog used to set application preferences
 */
class PrefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefDialog(QWidget *parent = 0);
    ~PrefDialog();

    const QStringList& getDarkSources() const { return _darkSources; }

protected:
    void changeEvent(QEvent *e);

private:
    Ui::PrefDialog *ui;

    QStringList _darkSources;

signals:
    void newDarkSources(const QStringList& paths);

private slots:
    void on_btnAddDarkFolder_clicked();
    void on_lstDarkFolders_itemSelectionChanged();
    void on_btnRemoveDarkFolder_clicked();
    virtual void accept();
};

#endif // PREFDIALOG_H
