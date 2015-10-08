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

#include "commands/commandManager.h"
#include "plotManager.h"


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
     * @param manager a pointer to the command manager
     * @param parent Qt parent
     */
    explicit MainWindow(CommandManager* manager, QWidget *parent = 0);

    /** Destructor */
    ~MainWindow();

protected:
    /**
     * Caught a change event
     * @param e the event
     */
    void changeEvent(QEvent *e);

private:
    /** the command manager */
    CommandManager* _commandManager;

    const QString LBL_DARKCOUNT_BASETEXT;
    const QString LBL_LIGHTSCOUNT_BASETEXT;

    /** Our UI */
    Ui::MainWindow *ui;

    /** update dark count indicator */
    void updateDarkContentCount();

    /** update lights count indicator */
    void updateLightsContentCount();

    /** Our datastore */
    DataStore* _dataStore;

    /** Our plot manager */
    PlotManager* _plotManager;

    void createProgressBarForCommand(AbstractCommand *command);

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

    /**
     * User double clicked a command log view cell
     *
     * @param index the model index displayed in this cell
     */
    void on_tblCommandView_doubleClicked(const QModelIndex & index);

    /**
     * User click a dark filter view item
     * @param index the clicked view index
     */
    void on_treeDarkView_clicked(const QModelIndex & index);

    /** user wants to clear the dark lib view filters */
    void on_btnDarkFilterClear_clicked();

    /** Dark list model changed */
    void on_darkListModelChanged();

    /** user triggered darkRescan action */
    void on_actionRescanDarksLibrary_triggered();

    /** User wants to change current lights folder */
    void on_btnChooseLightsFolder_clicked();

    /**
     * Dark sources have changed
     * @param sources a list of dark source folder paths
     */
    void on_darkSourcesChanged(const QStringList &sources);

    /** A dark scan just started */
    void on_darkScanStart();

    /** A dark scan just finished */
    void on_darkScanDone();

    /** A lights scan just started */
    void on_lightsScanStart();

    /** A lights scan just finished */
    void on_lightsScanDone();

    /** A lights consistency check result arrived */
    void on_consistencyResult(bool consistent);

    /** A new command has been created */
    void on_commandCreated(AbstractCommand* command);

signals:
    /** Tell the CommandManager to create a ScanDark command */
    void scanDarkLibrary();

    /** Tell the CommandManager to create a ScanLights command */
    void scanLightsFolder();

    /** Tell the CommandManager to create a ScanLights command */
    void checkLights();
};

#endif // MAINWINDOW_H
