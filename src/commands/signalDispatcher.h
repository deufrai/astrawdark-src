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

#ifndef SIGNALDISPATCHER_H
#define SIGNALDISPATCHER_H

#include <QObject>
#include <QStringList>

#include "data/imageInfo.h"
#include "abstractCommand.h"

/**
 * @brief Dispatches Qt Signals for all app objects.
 */
class SignalDispatcher : public QObject
{
    Q_OBJECT
private:
    /**
     * Constructor
     * @param parent Qt parent
     */
    explicit SignalDispatcher(QObject *parent = 0);

    /** The unique instance */
    static SignalDispatcher* _instance;

public:
    /**
     * Get the single instance
     * @return the instance
     */
    static SignalDispatcher* getInstance();

signals:
    /** A new dark scan has just started */
    void darkScanStarted();

    /** A lights scan has just started */
    void lightsScanStarted();

    /**
     * A dark scan has just finished
     * @param result a list of ImageInfo
     */
    void darkScanDone(QList<ImageInfo> result);

    /**
     * A lights scan has just finished
     * @param result a list of ImageInfo
     */
    void lightsScanDone(QList<ImageInfo> result);

    /**
     * a command has just been created
     * @param command the command
     */
    void commandCreated(AbstractCommand* command);

    /**
     * A command just changed its status
     * @param command the command
     */
    void commandStatusChange(AbstractCommand* command);

    /**
     * Dark sources have changed
     * @param sources a list of folder paths
     */
    void darkSourcesChanged(QStringList sources);

    /** let's create a dark scan command */
    void createDarkScanCommand();

    /** the dark list model has been updated */
    void darkListModelChanged();

    /** let's create a ligts scan command */
    void createLightsScanCommand();

    /** let's create a lights check command */
    void createLightsCheckCommand();

    /** advertize consistency check result */
    void consistencyResult(bool);

public slots:
    /** A new dark scan has just started */
    void on_darkScanStarted() {emit darkScanStarted();}

    /**
     * A dark scan has just finished
     * @param result a list of ImageInfo
     */
    void on_darkScanDone(QList<ImageInfo> result) {emit darkScanDone(result);}

    /**
     * A lights scan has just finished
     * @param result a list of ImageInfo
     */
    void on_lightsScanDone(QList<ImageInfo> result) {emit lightsScanDone(result);}

    /**
     * a command has just been created
     * @param command the command
     */
    void on_commandCreated(AbstractCommand* command) {emit commandCreated(command);}

    /**
     * A command just changed its status
     * @param command the command
     */
    void on_CommandStatusChange(AbstractCommand* command) {emit commandStatusChange(command);}

    /**
     * Dark sources have changed
     * @param sources a list of folder paths
     */
    void on_darkSourcesChanged(QStringList sources) { emit darkSourcesChanged(sources);}

    /** let's create a dark scan command */
    void on_createDarkScanCommand() {emit createDarkScanCommand();}

    /** dark list model changed */
    void on_darkListModelChanged() {emit darkListModelChanged();}

    /** let's create a lights scan command */
    void on_createLightsScanCommand() {emit createLightsScanCommand();}

    /** let's create a lights check command */
    void on_createLightsCheckCommand() {emit createLightsCheckCommand();}

    /** A new lights scan just started */
    void on_lightsScanStarted() {emit lightsScanStarted();}

    /** lights consistency result arrived */
    void on_consistencyResult(bool consistent) {emit consistencyResult(consistent);}



};

#endif // SIGNALDISPATCHER_H
