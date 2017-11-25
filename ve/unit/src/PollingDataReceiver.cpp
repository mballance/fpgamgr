/*
 * PollingDataReceiver.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#include "PollingDataReceiver.h"
#include <string.h>

PollingDataReceiver::PollingDataReceiver(IEventLoop *ev_loop) {
	m_cached_data = 0;
	m_cached_data_sz = 0;
	m_cached_data_max = 0;
	m_ev_loop = ev_loop;
}

PollingDataReceiver::~PollingDataReceiver() {
	// TODO Auto-generated destructor stub
}

size_t PollingDataReceiver::read(void *p, uint32_t sz) {
	size_t ret = 0;
	while (m_cached_data_sz == 0) {
		if (m_ev_loop->recv_and_dispatch() < 0) {
			break;
		}
	}

	if (m_cached_data_sz) {
		uint32_t cp_sz = (m_cached_data_sz >= sz)?sz:m_cached_data_sz;
		ret = cp_sz;

		memcpy(p, m_cached_data, cp_sz);

		if (m_cached_data_sz > cp_sz) {
			for (uint32_t i=cp_sz; i<m_cached_data_sz; i++) {
				m_cached_data[i-cp_sz] = m_cached_data[i];
			}
			m_cached_data_sz = (m_cached_data_sz-cp_sz);
		} else {
			m_cached_data_sz = 0;
		}
	}

	return ret;
}

// Callback that receives data from the other side
void PollingDataReceiver::write(const void *p, size_t sz) {
	if (m_cached_data_max < (m_cached_data_sz+sz)) {
		// resize
		uint8_t *t = m_cached_data;
		m_cached_data = new uint8_t[m_cached_data_sz+sz];
		if (m_cached_data_sz > 0) {
			memcpy(m_cached_data, t, m_cached_data_sz);
		}
		if (t) {
			delete [] t;
		}
	}

	memcpy(&m_cached_data[m_cached_data_sz], p, sz);
	m_cached_data_sz += sz;
}

void PollingDataReceiver::init(IDataStream *stream) {
	// TODO:
}
