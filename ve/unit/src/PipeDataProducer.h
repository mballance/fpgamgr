/*
 * PipeDataProducer.h
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef VE_UNIT_SRC_PIPEDATAPRODUCER_H_
#define VE_UNIT_SRC_PIPEDATAPRODUCER_H_
#include "IDataStream.h"
#include <stdint.h>
#include <pthread.h>

class PipeDataProducer : public IDataStream {
public:
	PipeDataProducer();

	virtual ~PipeDataProducer();

	virtual IDataStream::StreamType get_type() {
		return IDataStream::StreamType_Sideband;
	}

	virtual int open();

	virtual int close();

	virtual size_t read(void *data, size_t bytes);

	virtual size_t write(void *data, size_t bytes);

	virtual size_t do_write(const void *data, size_t bytes);

	virtual int get_fd();

private:
	int				m_fds[2];

};

#endif /* VE_UNIT_SRC_PIPEDATAPRODUCER_H_ */
