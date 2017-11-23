/*
 * FPGAMgrMsgStream.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrMsgStream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdio.h>

FPGAMgrMsgStream::FPGAMgrMsgStream(int fd) :
	m_fd(fd), m_buf_idx(0), m_buf_sz(0) {
	// TODO Auto-generated constructor stub

}

FPGAMgrMsgStream::~FPGAMgrMsgStream() { }

void FPGAMgrMsgStream::add_data_stream(
		IDataStream		*stream,
		IDataHandler	*handler) {
	m_data_streams.push_back(stream);
	m_data_handlers.push_back(handler);
}

void FPGAMgrMsgStream::set_msg_handler(
		IMsgHandler			*handler,
		uint8_t				ep) {
	if (ep >= m_msg_handlers.size()) {
		for (uint32_t i=m_msg_handlers.size(); i<=ep; i++) {
			m_msg_handlers.push_back(0);
		}
	}
	m_msg_handlers.at(ep) = handler;
}

bool FPGAMgrMsgStream::recv(FPGAMgrMsg &msg_r) {
	FPGAMgrMsg msg;

	while (true) {
		msg.reset_put();
		int c = get_c();

		if (c == 0x5a) { // marker byte
			uint8_t tmp[5];
			for (int i=0; i<5; i++) {
				c = get_c();
				if (c == -1) {
					fprintf(stdout, "Recv error\n");
					return false;
				}
				tmp[i] = c;
			}
			uint32_t sz =
					(tmp[0] << 0) |
					(tmp[1] << 8) |
					(tmp[2] << 16) |
					(tmp[3] << 24)
					;

			uint8_t ep = tmp[4]; // total message length

			if (!read(msg, sz)) {
				fprintf(stdout, "Recv error\n");
				return false;
			}

			if (ep == 0) {
				// Received a packet
				break;
			}
		} else {
			fprintf(stdout, "Error: missing marker byte\n");
			return false;
		}
	}

	msg_r = msg;

	return true;
}

bool FPGAMgrMsgStream::send(const FPGAMgrMsg &msg, uint8_t ep) {
	uint8_t *data = msg.data();
	uint32_t sz = msg.size();
	uint32_t i=0;

	uint8_t hdr[1+4+1]; // 1 marker + 4 size + 1 endpoint

	hdr[0] = 0x5A;
	hdr[1+0] = (sz >> 0);
	hdr[1+1] = (sz >> 8);
	hdr[1+2] = (sz >> 16);
	hdr[1+3] = (sz >> 24);
	hdr[5] = ep;

	::send(m_fd, hdr, 5, 0);

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

int FPGAMgrMsgStream::recv_and_dispatch(int32_t timeout_ms) {
	struct timeval t, *tp;
	int ret = 0;

	if (timeout_ms < 0) {
		tp = 0;
	} else {
		t.tv_sec = timeout_ms / 1000;
		t.tv_usec = (timeout_ms % 1000)*1000;
	}

	fd_set read_fds, excpt_fds;
	int32_t nfds = 0;

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

	fprintf(stdout, "--> select m_fd=%d nfds=%d\n", m_fd, (nfds+1));
	int sret = select(nfds+1, &read_fds, 0, 0, tp);
	fprintf(stdout, "<-- select ret=%d\n", ret);

	if (sret == -1) {
		return -1;
	} else {
		// First, check to see if any of the data-stream FDs are impacted
		for (uint32_t i=0; i<m_data_streams.size(); i++) {
			int32_t fd = m_data_streams.at(i)->get_fd();
			if (fd >= 0) {
				if (FD_ISSET(fd, &read_fds)) {
					int recv_ret = m_data_streams.at(i)->read(m_buf, sizeof(m_buf));

					if (recv_ret > 0) {
						// Pass the data on to the data handler
						m_data_handlers.at(i)->write(m_buf, recv_ret);
					}
				}
			}
		}
	}

	return sret;
}

int FPGAMgrMsgStream::get_c() {
	if (m_buf_idx >= m_buf_sz) {
		m_buf_sz = recv(m_buf, sizeof(m_buf));
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
			m_buf_sz = recv(m_buf, sizeof(m_buf));

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

size_t FPGAMgrMsgStream::recv(uint8_t *data, size_t sz) {

}
