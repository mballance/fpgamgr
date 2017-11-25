/*
 * SocketDataStream.h
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_SOCKETDATASTREAM_H_
#define SRC_COMMON_SOCKETDATASTREAM_H_

#include "IDataStream.h"
#include <stdint.h>

class SocketDataStream: public IDataStream {
public:
	SocketDataStream(int fd, IDataStream::StreamType type=IDataStream::StreamType_Core);

	virtual ~SocketDataStream();

	virtual IDataStream::StreamType get_type();

	virtual int open();

	virtual int close();

	virtual size_t read(void *data, size_t sz);

	virtual size_t write(void *data, size_t sz);

	virtual int get_fd();

private:
	int32_t						m_fd;
	IDataStream::StreamType		m_type;

};

#endif /* SRC_COMMON_SOCKETDATASTREAM_H_ */
