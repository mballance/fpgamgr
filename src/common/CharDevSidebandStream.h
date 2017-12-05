/*
 * CharDevSidebandStream.h
 *
 *  Created on: Dec 5, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_CHARDEVSIDEBANDSTREAM_H_
#define SRC_COMMON_CHARDEVSIDEBANDSTREAM_H_

#include "ISidebandStream.h"
#include <string>

class CharDevSidebandStream: public ISidebandStream {
public:
	CharDevSidebandStream(
			const std::string &path,
			IDataStream::StreamType type=IDataStream::StreamType_Sideband);

	virtual ~CharDevSidebandStream();

	virtual IDataStream::StreamType get_type();

	virtual int open();

	virtual int close();

	virtual size_t read(void *data, size_t size);

	virtual size_t write(void *data, size_t size);

	virtual bool message(uint8_t ep, const FPGAMgrMsg &msg);

	virtual int get_fd();

private:
	int							m_fd;
	std::string					m_path;
	IDataStream::StreamType		m_type;
};

#endif /* SRC_COMMON_CHARDEVSIDEBANDSTREAM_H_ */
