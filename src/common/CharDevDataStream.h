/*
 * CharDevDataStream.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_CHARDEVDATASTREAM_H_
#define SRC_COMMON_CHARDEVDATASTREAM_H_

#include "IDataStream.h"
#include <string>

class CharDevDataStream: public IDataStream {
public:
	CharDevDataStream(const std::string &path);

	virtual ~CharDevDataStream();

	virtual int open();

	virtual int close();

	virtual size_t read(void *data, size_t size);

	virtual size_t write(void *data, size_t size);

	virtual int get_fd();

private:
	int					m_fd;
	std::string			m_path;

};

#endif /* SRC_COMMON_CHARDEVDATASTREAM_H_ */
