/*
 * FPGAMgrMsgStream.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrMsgStream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

FPGAMgrMsgStream::FPGAMgrMsgStream(int fd) :
	m_fd(fd), m_buf_idx(0), m_buf_sz(0) {
	// TODO Auto-generated constructor stub

}

FPGAMgrMsgStream::~FPGAMgrMsgStream() {
	// TODO Auto-generated destructor stub
}

bool FPGAMgrMsgStream::recv(FPGAMgrMsg &msg_r) {
	FPGAMgrMsg msg;
	int c = get_c();

	if (c == 0x5a) { // marker byte
		for (int i=0; i<4; i++) {
			c = get_c();
			if (c == -1) {
				fprintf(stdout, "Recv error\n");
				return false;
			}
			msg.put8(c);
		}
		uint32_t sz = msg.get32(); // total message length

		if (!read(msg, sz)) {
			fprintf(stdout, "Recv error\n");
			return false;
		}
	} else {
		fprintf(stdout, "Error: missing marker byte\n");
		return false;
	}

	msg_r = msg;

	return true;
}

bool FPGAMgrMsgStream::send(const FPGAMgrMsg &msg) {
	uint8_t *data = msg.data();
	uint32_t sz = msg.size();
	uint32_t i=0;

	while (i < sz) {
		int ret = ::send(m_fd, &data[i], (sz-i), 0);

		if (ret > 0) {
			i += ret;
		} else {
			fprintf(stdout, "Send error\n");
			return false;
		}
	}

	return true;
}

int FPGAMgrMsgStream::get_c() {
	if (m_buf_idx >= m_buf_sz) {
		m_buf_sz = ::recv(m_fd, m_buf, sizeof(m_buf), 0);
		m_buf_idx = 0;
	}

	if (m_buf_idx < m_buf_sz) {
		return m_buf[m_buf_idx++];
	} else {
		return -1;
	}
}

bool FPGAMgrMsgStream::read(FPGAMgrMsg &msg, uint32_t sz) {
	while (sz) {
		if (m_buf_idx >= m_buf_sz) {
			m_buf_sz = ::recv(m_fd, m_buf, sizeof(m_buf), 0);

			if (m_buf_sz == 0) {
				fprintf(stdout, "Bulk recv fail\n");
				return false;
			}
			m_buf_idx = 0;
		}
		uint32_t cp_sz = ((m_buf_sz-m_buf_idx)<sz)?(m_buf_sz-m_buf_idx):sz;

		msg.write(&m_buf[m_buf_idx], cp_sz);
		m_buf_idx += cp_sz;

		sz -= cp_sz;
	}

	return true;
}
