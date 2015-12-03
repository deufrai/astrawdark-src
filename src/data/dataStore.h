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

#ifndef DATASTORE_H
#define DATASTORE_H

#include <QObject>
#include <QStandardItemModel>

#include "imageInfo.h"
#include "commands/abstractCommand.h"

/**
 * @brief Stores all application data.
 *
 * Implemented as a Singleton
 */
class DataStore : public QObject
{
    Q_OBJECT

private:
    /**
     * Default constructor
     */
    DataStore();

    /** The unique instance */
    static DataStore* _instance;
public:
    /**
     * Retrieve unique instance
     * @return the instance
     */
    static DataStore* getInstance();

    /**
     * Get dark list model
     * @return the model
     */
    QStandardItemModel* getDarkModel() const {return _darkListModel;}


    /**
     * Get lights list model
     * @return the model
     */
    QStandardItemModel* getLightsModel() const {return _lightsListModel;}

    /**
     * Get dark tree model
     * @return the model
     */
    QStandardItemModel* getDarkTreeModel() const {return _darkTreeModel;}

    /**
     * Get command list model
     * @return the model
     */
    QStandardItemModel* getCommandListModel() const {return _commandListModel;}

    /**
     * Get dark source folder paths
     * @return a list of paths
     */
    QStringList& getDarkSources() {return _darkSources;}

    /**
     * Does the user want the window to remember its geometry ?
     * @return true if user wants it
     */
    bool getRememberWindowGeometry() const {return _rememberWindowGeometry;}

    /**
     * Set the flag telling if user wants the window to remember its geometry
     * @param remember true or false
     */
    void setRememberWindowGeometry(bool remember);

    /**
     * Set the flag telling if user wants a dark lib scan on app startup
     * @param scan true or false
     */
    void setScanDarkOnStartup(bool scan);

    /**
     * Does the user want a dark lib scan on app startup ?
     * @return true if user wants it
     */
    bool getScanDarkOnStartup() const {return _scanDarksOnStartup;}

    /**
     * Set dark display filter
     * @param filter the filter string
     */
    void setDarkDisplayFilter(const QString filter);

    /**
     * Get the size of our dark library
     * @return dark libray size
     */
    int getDarkLibrarySize() const {return _scannedDarks.count();}

    /**
     * Get the size of lights model
     * @return lights model row count
     */
    int getLightsCount() const {return _scannedLights.count();}

    /**
     * Get all scanned darks
     * @return a list of darks infos
     */
    const QList<ImageInfo>& getScannedDarks() const {return _scannedDarks;}

    /**
     * Get all scanned lights
     * @return a list of lights infos
     */
    const QList<ImageInfo>& getScannedLights() const {return _scannedLights;}

    /**
     * Get all filtered darks
     * @return a list of darks infos
     */
    const QList<ImageInfo>& getFilteredDarks() const {return _filteredDarks;}

    /**
     * Get lights folder
     * @return lights folder
     */
    const QString& getLightsFolder() const {return _lightsFolder; }

    /**
     * Set lights folder
     * @param lights lights folder
     */
    void setLightsFolder(const QString lights) { _lightsFolder = lights;}

    /**
     * Get needed darks count for matching
     *
     * @return nedded darks count
     */
	int getNeededDarks4Matching() const { return _neededDarks4Matching; }

    /**
     * Set needed darks count for matching
     *
     * @param neededDarks4Matching nedded darks count
     */
	void setNeededDarks4Matching(int neededDarks4Matching) { _neededDarks4Matching = neededDarks4Matching; }

private:
    static QString      _S_DarkDisplayFilter;

    QStandardItemModel* _darkListModel;
    QStandardItemModel* _lightsListModel;
    QStandardItemModel* _commandListModel;
    QStandardItemModel* _darkTreeModel;
    QStringList         _darkSources;
    QList<ImageInfo>    _scannedDarks;
    QList<ImageInfo>    _filteredDarks;
    QList<ImageInfo>    _scannedLights;
    bool                _rememberWindowGeometry;
    bool                _scanDarksOnStartup;
    QString             _lightsFolder;
    int					_neededDarks4Matching;

    /**
     * Update an already known command
     *
     * @param row the model row to update
     * @param command the data to update it with
     */
    void updateCommandModelRow(int row, AbstractCommand* command);

    /**
     * Populate the dark filter treeView
     * @param imageInfos all scanned darks
     */
    void populateDarkFiltersTreeView(QList<ImageInfo> imageInfos);

    /**
     * Populate a frame model
     * @param model the model to populate
     * @param frames the frames to populate with
     */
    void populateFrameListModel(QStandardItemModel* model, QList<ImageInfo> frames);

    /**
     * Filter a dark
     * @param dark the dark to filter
     * @return true if dark is to be kept
     */
    static bool filterDark(ImageInfo dark);

    /**
     * Filter our dark list
     */
    void filterDarks();

signals:

    /**
     * Our dark list model has been updated;
     */
    void darkListModelChanged();

public slots:

    /** empty dark list model upon any new scan starts */
    void on_newDarkScanStarted();

    /** empty lights list model upon any new scan starts */
    void on_newLightsScanStarted();

    /**
     * Registers a new dark frame scan result
     * @param darks scan result
     */
    void on_newDarkScanResult(QList<ImageInfo> darks);

    /**
     * Registers a new lights scan result
     * @param lights scan result
     */
    void on_newLightsScanResult(QList<ImageInfo> lights);

    /**
     * Update model with changed command
     * @param command the command to update model with
     */
    void on_CommandStatusChange(AbstractCommand* command);

    /**
     * Update model wi newly created command
     * @param command the command to update model with
     */
    void on_CommandCreated(AbstractCommand* command);

    /**
     * Register new dark sources
     * @param paths a list of dark folder source paths
     */
    void on_newDarkSources(QStringList paths);

};

#endif // DATASTORE_H
