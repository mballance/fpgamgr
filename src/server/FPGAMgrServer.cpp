/*
 * FPGAMgrServer.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrServer.h"
#include "FPGAMgrMsgStream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "json.hpp"

using json = nlohmann::json;

FPGAMgrServer::FPGAMgrServer() : m_backend(0) {

}

FPGAMgrServer::~FPGAMgrServer() {
	// TODO Auto-generated destructor stub
}

void FPGAMgrServer::setBackend(IFPGABackend *backend) {
	m_backend = backend;
}

void FPGAMgrServer::addDataStream(IDataStream *ds) {

}

int FPGAMgrServer::start_server(int port) {
    struct sockaddr_in srv_addr;
	m_srv_sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = INADDR_ANY;
	srv_addr.sin_port = htons(port);

    if (bind(m_srv_sock, (struct sockaddr *) &srv_addr,
	              sizeof(srv_addr)) < 0) {
    	fprintf(stdout, "Error: failed to bind to srv_sock\n");
    }
    listen(m_srv_sock, 1);

    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);
    if (getsockname(m_srv_sock, (struct sockaddr *)&sin, &len) == -1) {
    	fprintf(stdout, "Error: failed to get socket name\n");
    }
        printf("port number %d\n", ntohs(sin.sin_port));

    m_srv_port = ntohs(sin.sin_port);

    return m_srv_port;
}

int FPGAMgrServer::run() {
    struct sockaddr_in cli_addr;

    while (true) {
    	FPGAMgrMsgStream *stream;
    	socklen_t cli_addr_len = sizeof(cli_addr);
    	int cli_sock = accept(m_srv_sock,
    			(struct sockaddr *)&cli_addr, &cli_addr_len);

    	stream = new FPGAMgrMsgStream(cli_sock);

    	while (true) {
    		std::string msg = stream->recv();
    		json jreq = json::parse(msg);
    		json jrsp;

    		std::string req = jreq["type"].get<std::string>();

    		if (req == "shutdown") {
    			jrsp["type"] = "OK";
    			stream->send(jrsp.dump());

    			// TODO: close streams, etc
    			return 0;
    		} else {
    			fprintf(stdout, "Error: unknown request \"%s\"\n", req.c_str());
    		}
    	}
    }

	return 0;
}

