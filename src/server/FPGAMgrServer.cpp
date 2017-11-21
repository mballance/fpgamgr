/*
 * FPGAMgrServer.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrServer.h"
#include "FPGAMgrMsgStream.h"
#include "FPGAMgrMsg.h"
#include "FPGAMgrMsgE.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>


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
		fprintf(stdout, "Note: waiting for connect\n");
    	int cli_sock = accept(m_srv_sock,
    			(struct sockaddr *)&cli_addr, &cli_addr_len);
		fprintf(stdout, "Note: connected\n");

    	stream = new FPGAMgrMsgStream(cli_sock);

    	while (true) {
    		FPGAMgrMsg req;
    		if (!stream->recv(req)) {
    			fprintf(stdout, "Broken pipe.\n");
    			delete stream;
    			stream = 0;
    			break;
    		}
    		FPGAMgrMsg rsp;
    		fpgamgr_msg_e msg_t = (fpgamgr_msg_e)req.get8();

    		if (msg_t == MSG_SHUTDOWN) {
    			rsp.put8(0x5a);
    			rsp.put32(1); // total length
    			rsp.put8(1); // OK
    			stream->send(rsp);

    			// TODO: close streams, etc
    			return 0;
    		} else if (msg_t == MSG_PROGRAM) {
    			uint32_t idx=0, id=1;
    			uint32_t total_sz = req.get32();
    			bool last = (bool)req.get8();
    			uint8_t *data = new uint8_t[total_sz];
   				FPGAMgrMsg rsp;

    			while (true) {
    				uint32_t this_sz = req.get32();
    				req.read(&data[idx], this_sz);

    				idx += this_sz;

    				if (!last) {
    					rsp.reset_put();
    					rsp.put8(0x5a);
    					rsp.put32(1); // total length
    					rsp.put8(1); // OK

    					if (!stream->send(rsp)) {
    						delete stream;
    						stream = 0;
    						break;
    					}

    					if (!stream->recv(req)) {
    						delete stream;
    						stream = 0;
    					}

    					req.get8(); // PROGRAM
    					req.get32(); // total_sz
    					last = (bool)req.get8();
    				} else {
    					break;
    				}
    			}

    			if (!stream) {
    				break;
    			}


    			int status = m_backend->program(data, idx);

    			// Acknowledge the final transfer
    			// TODO: detect if programming failed
   				rsp.reset_put();
   				rsp.put8(0x5a);
   				rsp.put32(1); // total length
   				rsp.put8((status == 0)); // OK
   				stream->send(rsp);

    			delete [] data;
    		} else if (msg_t == MSG_DISCONNECT) {
    			// TODO: block data from I/O streams
    			FPGAMgrMsg rsp;

    			rsp.put8(0x5a);
    			rsp.put32(1); // length
    			rsp.put8(1); // OK

    			stream->send(rsp);

    			fprintf(stdout, "Note: disconnect\n");
    			delete stream;
    			stream = 0;
    			break;
    		} else {
    			fprintf(stdout, "Error: unknown request %d\n", msg_t);
    		}
    	}
    }

	return 0;
}

