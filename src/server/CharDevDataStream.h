/*
 * CharDevDataStream.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_SERVER_CHARDEVDATASTREAM_H_
#define SRC_SERVER_CHARDEVDATASTREAM_H_

#include "IDataStream.h"

class CharDevDataStream: public IDataStream {
public:
	CharDevDataStream();

	virtual ~CharDevDataStream();

	virtual int open(const char *path);

	virtual size_t read(void *data, size_t size);

	virtual size_t write(void *data, size_t size);

	virtual int get_fd();

private:
	int					m_fd;

};

#endif /* SRC_SERVER_CHARDEVDATASTREAM_H_ */
