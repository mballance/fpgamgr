/*
 * FPGAMgrMsgStream.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrMsgStream.h"
#include <sys/types.h>
#include <sys/socket.h>

FPGAMgrMsgStream::FPGAMgrMsgStream(int fd) :
	m_fd(fd), m_buf_idx(0), m_buf_sz(0) {
	// TODO Auto-generated constructor stub

}

FPGAMgrMsgStream::~FPGAMgrMsgStream() {
	// TODO Auto-generated destructor stub
}

std::string FPGAMgrMsgStream::recv() {
	int c;
	std::string ret;

	while ((c = getc()) > 0) {
		ret.push_back(c);
	}

	return ret;
}

void FPGAMgrMsgStream::send(const std::string &msg) {
	const char *msg_c = msg.c_str();
	uint32_t sz = (msg.size()+1);
	uint32_t i=0;

	while (i < sz) {
		int ret = ::send(m_fd, &msg_c[i], (sz-i), 0);

		if (ret > 0) {
			i += ret;
		} else {
			break;
		}
	}
}

int FPGAMgrMsgStream::getc() {
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

