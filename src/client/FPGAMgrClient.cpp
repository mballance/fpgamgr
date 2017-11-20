/*
 * FPGAMgrClient.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrClient.h"
#include "FPGAMgrMsgStream.h"
#include "UUEncDec.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "json.hpp"

using json = nlohmann::json;

FPGAMgrClient::FPGAMgrClient() {
	// TODO Auto-generated constructor stub

}

FPGAMgrClient::~FPGAMgrClient() {
	// TODO Auto-generated destructor stub
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
	do {
		std::string data;
		uint32_t sz = fread(buf, 1, block_sz, fp);
		xmit_sz += sz;

		json req;
		req["type"] = "program";
		req["data"] = UUEncDec::encode(buf, sz, true);
		req["size"] = total_sz;
		req["last"] = (xmit_sz >= total_sz)?true:false;

		m_stream->send(req.dump());

		json rsp = json::parse(m_stream->recv());
	} while (xmit_sz < total_sz);

	delete [] buf;


	return 0;
}

int FPGAMgrClient::shutdown_server() {
	json req;

	req["type"] = "shutdown";

	m_stream->send(req.dump());

	json rsp = json::parse(m_stream->recv());

	return (rsp["type"] == "OK")?0:-1;
}

int FPGAMgrClient::close() {
	::close(m_client_sock);
	return 0;
}

