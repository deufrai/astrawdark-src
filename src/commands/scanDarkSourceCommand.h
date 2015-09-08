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

#include "abstractCommand.h"
#include "../data/imageInfo.h"

#include <string>
#include <QObject>

/**
 * @brief Command that fetches all dark frames from a directory.
 */
class ScanDarkSourceCommand : public QObject, public AbstractCommand
{
    Q_OBJECT
public:
    explicit ScanDarkSourceCommand(const QStringList& sources);
    virtual ~ScanDarkSourceCommand() {}

protected:
    virtual void do_processing();

private:
    const QStringList& _sources;

signals:
    void done(QList<ImageInfo> result);

public slots:

};

#endif // SCANDARKSOURCE_H
