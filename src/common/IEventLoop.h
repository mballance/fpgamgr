/*
 * IEventLoop.h
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_IEVENTLOOP_H_
#define SRC_COMMON_IEVENTLOOP_H_
#include <stdint.h>

class IEventLoop {
public:

	virtual ~IEventLoop() { }

	virtual int32_t recv_and_dispatch(int32_t timeout_ms=-1) = 0;
};




#endif /* SRC_COMMON_IEVENTLOOP_H_ */
