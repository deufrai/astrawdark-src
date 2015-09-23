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

#ifndef SCANLIGHTSCOMMAND_H
#define SCANLIGHTSCOMMAND_H

#include <QObject>

#include "abstractScanCommand.h"

/**
 * @brief Command that scans a unique lights directory
 */
class ScanLightsCommand : public AbstractScanCommand
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param dir the directory to scan for lights
     */
    ScanLightsCommand(const QString dir);

    /**
     * Destructor
     */
    virtual ~ScanLightsCommand() {}

    /**
     * The actual processing payload of this command.
     */
    virtual void do_processing();

signals:
    /** tell everyone we completed the scan */
    void scanDone(QList<ImageInfo> result);

    /** tell everyone we just started scanning */
    void scanStarted();

public slots:
};

#endif // SCANLIGHTSCOMMAND_H
