/*
 * FPGAMgrServer.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "FPGAMgrServer.h"
#include "SocketDataStream.h"
#include "FPGAMgrMsgDataHandler.h"
#include "SidebandDataHandler.h"
#include "FPGAMgrMsg.h"
#include "FPGAMgrMsgE.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

class ConnectDataStream : public IDataStream {
public:

	ConnectDataStream(
			FPGAMgrServer	*server,
			int32_t 		fd) : m_server(server), m_fd(fd) {
	}
	IDataStream::StreamType get_type() { return IDataStream::StreamType_Core; }

	virtual int open() { return 0; }

	virtual int close() { return 0; }

	virtual size_t read(void *data, size_t bytes) {
		m_server->handle_connect();
		return 0;
	}

	virtual size_t write(void *data, size_t bytes) { return 0; }

	virtual int get_fd() { return m_fd; }

private:
	FPGAMgrServer			*m_server;
	int32_t					m_fd;
};


FPGAMgrServer::FPGAMgrServer() :
	m_connect_stream(0),
	m_backend(0),
	m_sideband_open(false),
	m_srv_sock(-1),
	m_srv_port(0),
	m_shutdown_req(false),
	m_connected(false),
	m_program_data(0),
	m_program_data_idx(0),
	m_client_stream(0),
	m_msg_handler(0) {

}

FPGAMgrServer::~FPGAMgrServer() {
	if (m_connect_stream) {
		delete m_connect_stream;
	}
	// TODO Auto-generated destructor stub
}

void FPGAMgrServer::setBackend(IFPGABackend *backend) {
	m_backend = backend;
}

void FPGAMgrServer::set_sideband_stream(uint8_t ep, IDataStream *stream) {
	while (ep >= m_sideband_streams.size()) {
		m_sideband_streams.push_back(0);
	}

	m_sideband_streams.at(ep) = stream;
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

    m_connect_stream = new ConnectDataStream(this, m_srv_sock);
    init_wait_connect();

    return m_srv_port;
}

int FPGAMgrServer::run() {
	int32_t ret = 0;

    while (!m_shutdown_req) {
   		if (recv_and_dispatch() < 0) {
   			break;
   		}
    }

	return 0;
}

int32_t FPGAMgrServer::recv_and_dispatch(int32_t timeout_ms) {
	int32_t ret = m_muxdemux.recv_and_dispatch(timeout_ms);

	if (m_shutdown_req) {
		if (m_client_stream) {
			delete m_client_stream;
			m_client_stream = 0;
		}
		if (m_msg_handler) {
			delete m_msg_handler;
			m_msg_handler = 0;
		}
		ret = -1;
	}

	return ret;
}

bool FPGAMgrServer::message(uint8_t ep, const FPGAMgrMsg &msg) {
	FPGAMgrMsg req = msg;
	bool ret = true;

//	if (!recv(req)) {
//		fprintf(stdout, "Broken pipe.\n");
//		close_sideband_streams();
//
//		delete stream;
//		stream = 0;
//		break;
//	}
	FPGAMgrMsg rsp;
	fpgamgr_msg_e msg_t = (fpgamgr_msg_e)req.get8();

	switch (msg_t) {
	case MSG_SHUTDOWN: {
		rsp.put8(1); // OK
		m_msg_handler->message(0, rsp);

		// TODO: close streams, etc
		close_sideband_streams();

		m_shutdown_req = true;

		ret = false;
	} break;

	case MSG_PROGRAM: {
		uint32_t idx=0, id=1;
		uint32_t total_sz = req.get32();
		bool last = (bool)req.get8();
		uint32_t this_sz = req.get32();

		if (!m_program_data) {
			// Must be the first

			// First, close down all sideband streams
			close_sideband_streams();

			m_program_data = new uint8_t[total_sz];
			m_program_data_idx = 0;
		}

		req.read(&m_program_data[m_program_data_idx], this_sz);
		m_program_data_idx += this_sz;

		int32_t status = 0;

		if (last) {
			// Program the FPGA
			status = m_backend->program(m_program_data, m_program_data_idx);

			delete [] m_program_data;
			m_program_data = 0;
			m_program_data_idx = 0;
		}

		// Send a response to the packet
		rsp.reset_put();
		rsp.put8((status == 0)?1:0); // OK

		m_msg_handler->message(0, rsp);

//		if (status == 0) {
//			open_sideband_streams();
//		}
	} break;

	case MSG_DISCONNECT: {
		// TODO: block data from I/O streams
		FPGAMgrMsg rsp;

		// First, close down sideband channels (we're done)
		close_sideband_streams();

		rsp.put8(1); // OK
		m_msg_handler->message(0, rsp);

		// re-init for connection
		init_wait_connect();
	} break;

	case MSG_CONFIG_SIDEBAND: {
		uint8_t en = req.get8();

		fprintf(stdout, "CONFIG_SIDEBAND: en=%d enabled=%d\n",
				en, m_sideband_open);

		if (en) {
			open_sideband_streams();
		} else {
			close_sideband_streams();
		}

		rsp.reset_put();
		rsp.put8(1);

		m_msg_handler->message(0, rsp);
	} break;

	case MSG_PING: {
		FPGAMgrMsg rsp;

		rsp.put8(1); // OK

		m_msg_handler->message(0, rsp);
	} break;

	default:
		fprintf(stdout, "Error: unknown request %d\n", msg_t);
		break;
	}

	return true;
}

void FPGAMgrServer::close_sideband_streams() {
	if (m_sideband_open) {
		for (uint32_t i=1; i<m_sideband_streams.size(); i++) {
			if (m_sideband_streams.at(i)->get_type() == IDataStream::StreamType_Sideband) {
				m_sideband_streams.at(i)->close();
			}
		}
		m_sideband_open = false;
	}
}

void FPGAMgrServer::open_sideband_streams() {
	if (!m_sideband_open) {
		for (uint32_t i=1; i<m_sideband_streams.size(); i++) {
			if (m_sideband_streams.at(i)->get_type() == IDataStream::StreamType_Sideband) {
				m_sideband_streams.at(i)->open();
			}
		}
		m_sideband_open = true;
	}
}

void FPGAMgrServer::init_wait_connect() {
	if (m_client_stream) {
		delete m_client_stream;
		m_client_stream = 0;
	}
	if (m_msg_handler) {
		delete m_msg_handler;
		m_msg_handler = 0;
	}
	m_muxdemux.init();
	m_muxdemux.add_data_stream(m_connect_stream, 0);
}

void FPGAMgrServer::handle_connect() {
    struct sockaddr_in cli_addr;
	socklen_t cli_addr_len = sizeof(cli_addr);

	fprintf(stdout, "Note: handle_connect\n");

	int cli_sock = accept(m_srv_sock,
			(struct sockaddr *)&cli_addr, &cli_addr_len);

	fprintf(stdout, "Note: connected\n");

	// Re-initialize the mux/demux
	m_muxdemux.init();

	// TODO: do we need an initialization message?
	// For example, to state whether streams should be connected

	SocketDataStream *stream = new SocketDataStream(cli_sock);
	FPGAMgrMsgDataHandler *handler = new FPGAMgrMsgDataHandler();
	handler->set_msg_handler(0, this);
	m_muxdemux.add_data_stream(stream, handler);

	m_client_stream = stream;
	m_msg_handler = handler;

	// Register all the data handlers
	// TODO: do this after connection?
	for (uint32_t i=1; i<m_sideband_streams.size(); i++) {
		SidebandDataHandler *sb_handler = new SidebandDataHandler(i, handler);
		m_muxdemux.add_data_stream(
				m_sideband_streams.at(i), sb_handler);
		handler->set_msg_handler(i, sb_handler);
	}
}

