/*
 * FPGAMgrMsgDataHandler.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: ballance
 */

#include "FPGAMgrMsgDataHandler.h"
#include <stdio.h>

FPGAMgrMsgDataHandler::FPGAMgrMsgDataHandler() {
	m_recv_state = RS_MARKER;
	m_hdr_idx = 0;
	m_data_sz = 0;
	m_ep = 0;
	m_stream = 0;
}

FPGAMgrMsgDataHandler::~FPGAMgrMsgDataHandler() {
	// TODO Auto-generated destructor stub
}

void FPGAMgrMsgDataHandler::init(IDataStream *stream) {
	m_stream = stream;
}

void FPGAMgrMsgDataHandler::write(void *p, size_t sz) {
	uint8_t *data = static_cast<uint8_t *>(p);

	for (uint32_t i=0; i<sz; i++) {
		switch (m_recv_state) {
		case RS_MARKER: {
			if (data[i] == 0x5a) {
				m_recv_state = RS_HEADER;
				m_hdr_idx = 0;
			}
		} break;

		case RS_HEADER: {
			m_hdr[m_hdr_idx++] = data[i];

			if (m_hdr_idx >= 5) {
				m_data_sz = (
						m_hdr[0] |
						(m_hdr[1] << 8) |
						(m_hdr[2] << 16) |
						(m_hdr[3] << 24)
						);
				m_ep = m_hdr[4];
				m_recv_state = RS_DATA;
				m_msg.reset_put();
			}
		} break;

		case RS_DATA: {
			m_msg.put8(data[i]);
			if (m_msg.size() >= m_data_sz) {
				// TODO: send message
				if (m_ep < m_msg_handlers.size() &&
						m_msg_handlers.at(m_ep)) {
					m_msg_handlers.at(m_ep)->message(m_ep, m_msg);
				} else {
					fprintf(stdout, "Error: no handler for message on EP %d\n", m_ep);
				}
				m_recv_state = RS_MARKER;
			}
		} break;
		}

	}

}

void FPGAMgrMsgDataHandler::set_msg_handler(
		uint32_t		id,
		IMsgHandler		*handler) {
	if (id >= m_msg_handlers.size()) {
		for (uint32_t i=m_msg_handlers.size(); i<=id; i++) {
			m_msg_handlers.push_back(0);
		}
	}
	m_msg_handlers.at(id) = handler;
}

bool FPGAMgrMsgDataHandler::message(uint8_t ep, const FPGAMgrMsg &msg) {
	// Send the message with the specified EP
	uint8_t *data = msg.data();
	uint32_t sz = msg.size();
	uint32_t i=0;

	uint8_t hdr[1+4+1]; // 1 marker + 4 size + 1 EP

	hdr[0] = 0x5A;
	hdr[1+0] = (sz >> 0);
	hdr[1+1] = (sz >> 8);
	hdr[1+2] = (sz >> 16);
	hdr[1+3] = (sz >> 24);
	hdr[5] = ep;

	// TODO: lock
	int ret;
	if ((ret=m_stream->write(hdr, 6)) != 6) {
		fprintf(stdout, "write returned %d instead of 5\n", ret);
		return false;
	}

	while (i < sz) {
		int ret = m_stream->write(&data[i], (sz-i));

		if (ret > 0) {
			i += ret;
		} else {
			return false;
		}
	}
	// TODO: unlock

	return true;
}
