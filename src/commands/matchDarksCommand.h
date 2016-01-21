/*
 * matchDarksCommand.h
 *
 *  Created on: 21 janv. 2016
 *      Author: deufrai
 */

#ifndef SRC_COMMANDS_MATCHDARKSCOMMAND_H_
#define SRC_COMMANDS_MATCHDARKSCOMMAND_H_

#include "abstractCommand.h"

/**
 * @brief Command that creates a list of darks
 * from a list of lights
 *
 * match T° distribution as close as possible between the 2 image lists
 */
class MatchDarksCommand : public AbstractCommand {

	Q_OBJECT
public:
	MatchDarksCommand();
	virtual ~MatchDarksCommand();

protected:
    /** processing payload */
    virtual void do_processing();
};

#endif /* SRC_COMMANDS_MATCHDARKSCOMMAND_H_ */
