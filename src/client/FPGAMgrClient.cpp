/*
 * FPGAMgrClient.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrClient.h"
#include "FPGAMgrMsgStream.h"
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

int FPGAMgrClient::shutdown_server() {
	json req;

	req["type"] = "shutdown";

	m_stream->send(req.dump());

	json rsp = json::parse(m_stream->recv());
	fprintf(stdout, "shutdown: ack=%s\n",
			rsp["type"].get<std::string>().c_str());

	return 0;
}

int FPGAMgrClient::close() {
	::close(m_client_sock);
	return 0;
}

