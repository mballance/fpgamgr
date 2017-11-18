/*
 * IDataStream.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_SERVER_IDATASTREAM_H_
#define SRC_SERVER_IDATASTREAM_H_
#include <sys/types.h>

class IDataStream {
public:

	virtual ~IDataStream() { }

	virtual size_t read(void *data, size_t bytes) = 0;

	virtual size_t write(void *data, size_t bytes) = 0;

	virtual int get_fd() = 0;

};




#endif /* SRC_SERVER_IDATASTREAM_H_ */
