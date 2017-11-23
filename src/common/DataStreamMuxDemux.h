/*
 * DataStreamMuxDemux.h
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_DATASTREAMMUXDEMUX_H_
#define SRC_COMMON_DATASTREAMMUXDEMUX_H_
#include <stdint.h>
#include <vector>
#include "IDataStream.h"
#include "IDataHandler.h"

class DataStreamMuxDemux {
public:
	DataStreamMuxDemux();

	virtual ~DataStreamMuxDemux();

	void init();

	void add_data_stream(
			IDataStream			*stream,
			IDataHandler		*handler);

	int recv_and_dispatch(int32_t timeout_ms=-1);

private:
	uint8_t							m_buf[4096];
	std::vector<IDataStream *>		m_data_streams;
	std::vector<IDataHandler *>		m_data_handlers;
};

#endif /* SRC_COMMON_DATASTREAMMUXDEMUX_H_ */
