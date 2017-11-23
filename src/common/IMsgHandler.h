/*
 * IMsgHandler.h
 *
 *  Created on: Nov 22, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_IMSGHANDLER_H_
#define SRC_COMMON_IMSGHANDLER_H_
#include "FPGAMgrMsg.h"
#include <stdint.h>

class IMsgHandler {
public:

	virtual ~IMsgHandler() { }

	virtual bool message(uint8_t ep, const FPGAMgrMsg &msg) = 0;
};



#endif /* SRC_COMMON_IMSGHANDLER_H_ */
