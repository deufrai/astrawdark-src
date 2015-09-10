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
 *
 * Provides generic ways to interract with any command type
 */
class AbstractCommand: public QObject
{
    Q_OBJECT

protected:
    /**
     * Default constructor
     */
    AbstractCommand();
public:
    /**
     * Default destructor
     */
    virtual ~AbstractCommand();

    /**
     * The Status enum describes a command's running status
     */
    enum Status {

        SCHEDULED,
        RUNNING,
        COMPLETE
    };

public:
    /**
     * execute command payload
     */
    void execute();

    /**
     * get command's description displayed in command log
     * @return the command's description
     */
    const QString getDescription() const {return _description;}

    /**
     * get command's running status
     * @return the status
     */
    Status getStatus() const {return _status;}

    /**
     * Get command's total execution time
     * @return elapsed time
     */
    qint64 getElapsed() const {return _elapsed;}

    /**
     * Get command's serial number.
     *
     * This serial number starts at 0 and incremented by 1 upon every
     * command construction.
     *
     * @return the serial
     */
    quint64 getSerial() const {return _serial;}

    /**
     * Tells if this command encountered errors during execution.
     * @return true if errors were encountered
     */
    bool hasErrors() const {return _error;}

    /**
     * Get command's error message
     * @return the error message
     */
    QString getErrorMessage() const {return _errorMessage;}

    /**
     * Get command's progress message.
     *
     * This message is displayed in command log
     * @return the progress message
     */
    QString getProgessMessage() const {return _progressMessage;}


private:
    /** The serial number */
    static quint64 SERIAL;

protected:
    /**
     * Any subclass can redefine this function to make any necessary
     * preparation before acutal payload processing.
     */
    virtual void setup() {}
    /**
     * Any subclass can redefine this function to make any necessary
     * cleanup before acutal payload processing.
     */
    virtual void cleanup() {}

    /**
     * The actual processing payload of this command.
     * All sublasses MUST implement this function.
     */
    virtual void do_processing() = 0;

    /** Command description */
    QString         _description;
    /** Command progress message */
    QString         _progressMessage;
    /** Command error message */
    QString         _errorMessage;
    /** Command running status */
    Status          _status;
    /** Timer used to get total running time */
    QElapsedTimer   _timer;
    /** Command total running time */
    qint64          _elapsed;
    /** Command serial number */
    quint64         _serial;
    /** Flag used to tell if command encountered error during payload processing */
    bool            _error;

signals:
    /** Advertise status changes */
    void statusChanged(AbstractCommand*);

};

#endif // ABSTRACTCOMMAND_H
