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

#ifndef COMMANDREPORTDISPLAY_H
#define COMMANDREPORTDISPLAY_H

#include <QDialog>

class AbstractCommand;

namespace Ui {
    class CommandReportDisplay;
}

/**
 * @brief widget to display full info on an AbstractCommand
 */
class CommandReportDisplay : public QDialog
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param command the command we are displaying
     * @param parent Qt parent
     */
    explicit CommandReportDisplay(const AbstractCommand* command, QWidget *parent = 0);

    /** Destructor */
    ~CommandReportDisplay();

protected:
    /**
     * We saw a change event
     */
    void changeEvent(QEvent *e);

private:
    /** our UI */
    Ui::CommandReportDisplay *ui;

    /** The command we are displaying */
    const AbstractCommand* _command;

    /** update commande display */
    void updateDisplay();

public slots:
    /**
     * calls updateDisplay() is our command matches the signaled one
     * @param command the signaled command
     */
    void on_commandUpdate(AbstractCommand* command);
};

#endif // COMMANDREPORTDISPLAY_H
