/*
 * FPGAMgrClient.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrClient.h"
#include "FPGAMgrMsg.h"
#include "FPGAMgrMsgE.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "SocketDataStream.h"
#include "FPGAMgrMsgDataHandler.h"

FPGAMgrClient::FPGAMgrClient() :
	m_have_msg(false),
	m_send(0),
	m_stream(0),
	m_handler(0) {

}

FPGAMgrClient::~FPGAMgrClient() {
	if (m_stream) {
		delete m_stream;
	}
	if (m_handler) {
		delete m_handler;
	}
}

void FPGAMgrClient::set_sideband_handler(uint8_t ep, IDataHandler *handler) {
	m_handler->set_msg_handler(ep, this);

	for (int32_t i=m_sideband_handlers.size(); i<=ep; i++) {
		m_sideband_handlers.push_back(0);
	}
	m_sideband_handlers.at(ep) = handler;
}

int FPGAMgrClient::connect(const std::string &host, uint16_t port) {
	char port_s[64];
	struct addrinfo hints;
	struct addrinfo *addr;
	struct sockaddr_in srv_addr;

	::memset(&hints, 0, sizeof(addrinfo));

	::sprintf(port_s, "%d", port);

	::getaddrinfo(host.c_str(), port_s, 0, &addr);

	int32_t client_sock = ::socket(AF_INET, SOCK_STREAM, 0);

	if (::connect(client_sock, addr->ai_addr, addr->ai_addrlen) != 0) {
		fprintf(stdout, "Error: connect failed\n");
		return -1;
	}

	m_stream = new SocketDataStream(client_sock);
	FPGAMgrMsgDataHandler *handler = new FPGAMgrMsgDataHandler();
	handler->set_msg_handler(0, this);
	m_send = handler;
	m_handler = handler;

	m_muxdemux.add_data_stream(m_stream, m_handler);

	// TODO: connect other data handlers

	freeaddrinfo(addr);

	return 0;
}

int32_t FPGAMgrClient::recv_and_dispatch(int32_t timeout) {
	return m_muxdemux.recv_and_dispatch(timeout);
}

int FPGAMgrClient::program(const std::string &path) {
	FILE *fp = fopen(path.c_str(), "rb");

	if (!fp) {
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	uint32_t total_sz = ftell(fp);
	fseek(fp, 0 ,SEEK_SET);
	uint32_t xmit_sz = 0;

	const uint32_t block_sz = 256 * 1024;

	uint8_t *buf = new uint8_t[block_sz];
	uint8_t ok;
	FPGAMgrMsg req;
	do {
		std::string data;
		uint32_t sz = fread(buf, 1, block_sz, fp);
		req.reset_put();
		xmit_sz += sz;

		req.put8(MSG_PROGRAM);
		req.put32(total_sz); // send the total size
		req.put8((xmit_sz >= total_sz));
		req.put32(sz); // This size
		req.write(buf, sz);

		m_send->message(0, req);

		FPGAMgrMsg rsp;
		if (!recv(rsp)) {
			return -1;
		}
		ok = rsp.get8();

	} while (xmit_sz < total_sz);

	delete [] buf;

	return (ok == 1)?0:-1;
}

int FPGAMgrClient::enable_sideband() {
	FPGAMgrMsg req;

	req.put8(MSG_CONFIG_SIDEBAND);
	req.put8(1);

	if (!m_send->message(0, req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!recv(rsp)) {
		return -1;
	}

	return (rsp.get8() == 1)?0:-1;
}

int FPGAMgrClient::disable_sideband() {
	FPGAMgrMsg req;

	req.put8(MSG_CONFIG_SIDEBAND);
	req.put8(0);

	if (!m_send->message(0, req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!recv(rsp)) {
		return -1;
	}

	return (rsp.get8() == 1)?0:-1;
}

int FPGAMgrClient::shutdown_server() {
	FPGAMgrMsg req;

	req.put8(MSG_SHUTDOWN);

	if (!m_send->message(0, req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!recv(rsp)) {
		return -1;
	}

	m_muxdemux.init(); // Clear references
	delete m_stream;
	m_stream = 0;
	delete m_handler;
	m_handler = 0;

	return (rsp.get8() == 1)?0:-1;
}

int FPGAMgrClient::close() {
	// Send a disconnect message
	FPGAMgrMsg req;

	if (!m_stream) {
		return -1;
	}

	req.put8(MSG_DISCONNECT);

	if (!m_send->message(0, req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!recv(rsp)) {
		return -1;
	}

	m_muxdemux.init(); // Clear references
	delete m_stream;
	m_stream = 0;
	delete m_handler;
	m_handler = 0;

	return (rsp.get8() == 1)?0:-1;
}

bool FPGAMgrClient::message(uint8_t ep, const FPGAMgrMsg &msg) {
	if (ep == 0) {
		m_have_msg = true;
		m_msg = msg;
	} else {
		if (m_sideband_handlers.size() > ep && m_sideband_handlers.at(ep)) {
			const uint8_t *data = msg.data();
			uint32_t sz = msg.size();
			m_sideband_handlers.at(ep)->write(data, sz);
		} else {
			fprintf(stdout, "Error: no sideband handler registered for EP %d\n", ep);
		}
	}

	return true;
}

bool FPGAMgrClient::recv(FPGAMgrMsg &msg) {
	bool ret = false;

	while (!m_have_msg) {
		if (m_muxdemux.recv_and_dispatch() < 0) {
			break;
		}
	}

	ret = m_have_msg;
	m_have_msg = false;
	msg = m_msg;

	return ret;
}
