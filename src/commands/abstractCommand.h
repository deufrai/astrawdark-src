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

#ifndef ABSTRACTCOMMAND_H
#define ABSTRACTCOMMAND_H

#include <QString>
#include <QObject>
#include <QElapsedTimer>

/**
 * @brief Interface for all commands.
 */
class AbstractCommand: public QObject
{
    Q_OBJECT
protected:
    AbstractCommand();
public:
    virtual ~AbstractCommand();

    enum Status {

        PENDING,
        RUNNING,
        COMPLETE
    };

public:
    void execute();
    const QString getDescription() const {return _description;}
    Status getStatus() const {return _status;}
    qint64 getElapsed() const {return _elapsed;}
    quint64 getSerial() const {return _serial;}
    bool hasErrors() const {return _error;}
    QString getErrorMessage() const {return _errorMessage;}

private:
    static quint64 SERIAL;

protected:
    virtual void setup() {}
    virtual void cleanup() {}
    virtual void do_processing() = 0;

    QString _description;
    QString _errorMessage;
    Status _status;
    QElapsedTimer _timer;
    qint64 _elapsed;
    quint64 _serial;
    bool    _error;

signals:
    void statusChanged(AbstractCommand*);

};

#endif // ABSTRACTCOMMAND_H
