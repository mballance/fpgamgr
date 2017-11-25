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
	CharDevDataStream(const std::string &path, IDataStream::StreamType type=IDataStream::StreamType_Sideband);

	virtual ~CharDevDataStream();

	virtual IDataStream::StreamType get_type();

	virtual int open();

	virtual int close();

	virtual size_t read(void *data, size_t size);

	virtual size_t write(void *data, size_t size);

	virtual int get_fd();

private:
	int							m_fd;
	std::string					m_path;
	IDataStream::StreamType		m_type;

};

#endif /* SRC_COMMON_CHARDEVDATASTREAM_H_ */
