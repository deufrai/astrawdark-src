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

class QListWidgetItem;

/**
 * @brief Dialog used to set application preferences
 */
class PrefDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Qt parent
     */
    explicit PrefDialog(QWidget *parent = 0);

    /** Desctructor */
    ~PrefDialog();

    /**
     * Get dark sources
     * @return a list of dark source folder paths
     */
    const QStringList& getDarkSources() const { return _darkSources; }

protected:
    /**
     * Caught a change event
     * @param e the event
     */
    void changeEvent(QEvent *e);

private:
    /** Our UI */
    Ui::PrefDialog *ui;

    /** Dark sources */
    QStringList _darkSources;

    /**
     * Check if a path can be used as dark frame source.
     *
     * @param path a  path
     * @return true or false
     */
    bool canPathBeUsedAsDarkSource(const QString& path);

signals:
    /**
     * Dark sources have changed
     * @param paths a list of dark source folder paths
     */
    void newDarkSources(const QStringList& paths);

    /**
     * We want a new dark scan now
     */
    void createDarkScanCommand();

private slots:
    /** User clicked the 'Add' button in dark source setting section */
    void on_btnAddDarkFolder_clicked();

    /** User selected a list item in dark source setting section */
    void on_lstDarkFolders_itemSelectionChanged();

    /** User clicked the 'Remove' button in dark source setting section */
    void on_btnRemoveDarkFolder_clicked();

    /** User clicked the 'Edit' button in dark source setting section */
    void on_btnEditDarkFolder_clicked();

    /** User double-clicked a list item in dark source setting section */
    void on_lstDarkFolders_itemDoubleClicked(QListWidgetItem * item);

    /** User clicked the global OK button */
    virtual void accept();
};

#endif // PREFDIALOG_H
