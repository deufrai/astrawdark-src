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

#ifndef ABSTRACTSCANCOMMAND_H
#define ABSTRACTSCANCOMMAND_H

#include <QObject>

#include "abstractCommand.h"
#include "../data/imageInfo.h"

/**
 * @brief Base of all folder scanning commands
 */
class AbstractScanCommand : public AbstractCommand
{
    Q_OBJECT

protected:
    /**
     * Constructor
     * @param sources dark source folder paths
     */
    AbstractScanCommand(const QStringList sources);

    /**
     * Get a list of Canon RAW file paths located inside a directory
     *
     * @param directory the directory
     */
    void getRawPathsInDirectory(const QString directory);

    /**
     * Retrieve EXIF metadata for a list of file path
     */
    void retrieveExifMetadata();

public:
    /**
     * Destructor
     */
    virtual ~AbstractScanCommand();

protected:

    /** check for scan errors */
    void checkForErrors();

    /** Dark source folder paths */
    const QStringList   _sources;
    /** Paths of invalid files */
    QStringList         _badFilesPaths;
    /** Paths of short exposure files */
    QStringList         _shortFilesPaths;
    /** A list of RAW file paths */
    QList<QString>      _imagePaths;
    /** A list of ImageInfo, result of EXIF metadata retrieval */
    QList<ImageInfo>    _imageInfos;

public slots:
};

#endif // ABSTRACTSCANCOMMAND_H
