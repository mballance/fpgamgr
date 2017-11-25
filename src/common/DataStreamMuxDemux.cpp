/*
 * DataStreamMuxDemux.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#include "DataStreamMuxDemux.h"
#include <stdio.h>

DataStreamMuxDemux::DataStreamMuxDemux() {
	// TODO Auto-generated constructor stub

}

DataStreamMuxDemux::~DataStreamMuxDemux() {
	// TODO Auto-generated destructor stub
}

void DataStreamMuxDemux::init() {
	m_data_handlers.clear();
	m_data_streams.clear();
}

void DataStreamMuxDemux::add_data_stream(
		IDataStream		*stream,
		IDataHandler	*handler) {
	m_data_streams.push_back(stream);
	m_data_handlers.push_back(handler);

	if (handler) {
		handler->init(stream);
	}
}

int DataStreamMuxDemux::recv_and_dispatch(int32_t timeout_ms) {
	struct timeval t, *tp;
	int ret = 0;

	if (timeout_ms < 0) {
		tp = 0;
	} else {
		t.tv_sec = timeout_ms / 1000;
		t.tv_usec = (timeout_ms % 1000)*1000;
		tp = &t;
	}

	fd_set read_fds, excpt_fds;
	int32_t nfds = -1;

	FD_ZERO(&read_fds);
	FD_ZERO(&excpt_fds);

	for (uint32_t i=0; i<m_data_streams.size(); i++) {
		int32_t fd = m_data_streams.at(i)->get_fd();
		if (fd > 0) {
			FD_SET(fd, &read_fds);
//			FD_SET(fd, &excpt_fds);
			if (fd > nfds) {
				nfds = fd;
			}
		}
	}

	if (nfds < 0) {
		// Nothing to do
		return -1;
	}

	int sret = select(nfds+1, &read_fds, 0, 0, tp);

	if (sret == -1) {
		return -1;
	} else {
		// First, check to see if any of the data-stream FDs are impacted
		for (uint32_t i=0; i<m_data_streams.size(); i++) {
			int32_t fd = m_data_streams.at(i)->get_fd();
			if (fd >= 0) {
				if (FD_ISSET(fd, &read_fds)) {
					int recv_ret = m_data_streams.at(i)->read(m_buf, sizeof(m_buf));

					if (recv_ret > 0 && m_data_handlers.at(i)) {
						// Pass the data on to the data handler
						m_data_handlers.at(i)->write(m_buf, recv_ret);
					}
				}
			}
		}
	}

	return sret;
}

