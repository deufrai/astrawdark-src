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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {

class MainWindow;
}

/**
 * @brief Our application's main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param parent Qt parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /** Destructor */
    ~MainWindow();

protected:
    /**
     * Caught a change event
     * @param e the event
     */
    void changeEvent(QEvent *e);

private:
    /** Our UI */
    Ui::MainWindow *ui;

private slots:
    /** User wants to quit */
    void on_actionQuit_triggered();

    /** User wants a dark library scan */
    void on_btnRescanDarks_clicked();

    /** User wants to see preference dialog */
    void on_actionPrefs_triggered();

    /**
     * Command log display menu item state has changed
     *
     * @param checked true if menu item is checked
     */
    void on_actionCommandLog_toggled(bool checked);

    /** User wants to see About dialog */
    void on_actionAbout_triggered();

public slots:
    /**
     * Dark sources have changed
     * @param sources a list of dark source folder paths
     */
    void on_darkSourcesChanged(const QStringList &sources);

    /** A dark scan just started */
    void on_darkScanStart();

    /** A dark scan just finished */
    void on_darkScanDone();

signals:
    /** Tell the CommandManager to create a ScanDark command */
    void scanDarkLibrary();
};

#endif // MAINWINDOW_H
