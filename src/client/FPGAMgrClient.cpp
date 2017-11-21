/*
 * FPGAMgrClient.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrClient.h"
#include "FPGAMgrMsgStream.h"
#include "FPGAMgrMsg.h"
#include "FPGAMgrMsgE.h"
#include "UUEncDec.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

FPGAMgrClient::FPGAMgrClient() :
	m_client_sock(-1), m_stream(0) {
	// TODO Auto-generated constructor stub

}

FPGAMgrClient::~FPGAMgrClient() {
	if (m_client_sock != -1) {
		::close(m_client_sock);
		m_client_sock = -1;
	}
	if (m_stream) {
		delete m_stream;
	}
}

int FPGAMgrClient::connect(const std::string &host, uint16_t port) {
	char port_s[64];
	struct addrinfo hints;
	struct addrinfo *addr;
	struct sockaddr_in srv_addr;

	::memset(&hints, 0, sizeof(addrinfo));

	::sprintf(port_s, "%d", port);

	::getaddrinfo(host.c_str(), port_s, 0, &addr);

	m_client_sock = ::socket(AF_INET, SOCK_STREAM, 0);

	if (::connect(m_client_sock, addr->ai_addr, addr->ai_addrlen) != 0) {
		fprintf(stdout, "Error: connect failed\n");
		return -1;
	}

	m_stream = new FPGAMgrMsgStream(m_client_sock);

	freeaddrinfo(addr);

	return 0;
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

		req.put8(0x5a); // marker byte
		req.put32(sz+1+4+1+4); // total size of this message
		req.put8(MSG_PROGRAM);
		req.put32(total_sz); // send the total size
		req.put8((xmit_sz >= total_sz));
		req.put32(sz); // This size
		req.write(buf, sz);

		m_stream->send(req);

		FPGAMgrMsg rsp;
		if (!m_stream->recv(rsp)) {
			return -1;
		}
		ok = rsp.get8();

	} while (xmit_sz < total_sz);

	delete [] buf;

	return (ok == 1)?0:-1;
}

int FPGAMgrClient::shutdown_server() {
	FPGAMgrMsg req;

	req.put8(0x5A); // marker byte
	req.put32(1); // message total size
	req.put8(MSG_SHUTDOWN);

	if (!m_stream->send(req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!m_stream->recv(rsp)) {
		return -1;
	}

	return (rsp.get8() == 1)?0:-1;
}

int FPGAMgrClient::close() {
	// Send a disconnect message
	FPGAMgrMsg req;

	req.put8(0x5A); // marker byte
	req.put32(1); // message total size
	req.put8(MSG_DISCONNECT);

	if (!m_stream->send(req)) {
		return -1;
	}

	FPGAMgrMsg rsp;
	if (!m_stream->recv(rsp)) {
		return -1;
	}

	::close(m_client_sock);
	m_client_sock = -1;
	return 0;
}

