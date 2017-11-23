/*
 * IDataHandler.h
 *
 *  Created on: Nov 22, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_IDATAHANDLER_H_
#define SRC_COMMON_IDATAHANDLER_H_
#include <sys/types.h>
#include "IDataStream.h"

/**
 * Interface class for handlers of data from FD streams
 */
class IDataHandler {
public:
	virtual ~IDataHandler() { }

	virtual void init(IDataStream *stream) = 0;

	virtual void write(void *data, size_t sz) = 0;
};



#endif /* SRC_COMMON_IDATAHANDLER_H_ */
