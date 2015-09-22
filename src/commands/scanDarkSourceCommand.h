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

#ifndef SCANDARKSOURCE_H
#define SCANDARKSOURCE_H

#include "abstractScanCommand.h"

/**
 * @brief Command that fetches all dark frames metadata
 */
class ScanDarkSourceCommand : public AbstractScanCommand
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param sources dark source folder paths
     */
    explicit ScanDarkSourceCommand(const QStringList& sources);

    /**
     * Destructor
     */
    virtual ~ScanDarkSourceCommand() {}

protected:
    /** processing payload */
    virtual void do_processing();

private:
    /** Paths of missing dark source folders */
    QStringList      _missingDirsPaths;

    /** check for scan errors */
    void checkForErrors();

signals:
    /** tell everyone we completed the scan */
    void scanDone(QList<ImageInfo> result);

    /** tell everyone we just started scanning */
    void scanStarted();

public slots:

};

#endif // SCANDARKSOURCE_H
