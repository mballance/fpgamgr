/*
 * FPGAMgrServer.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_SERVER_FPGAMGRSERVER_H_
#define SRC_SERVER_FPGAMGRSERVER_H_
#include "IFPGABackend.h"
#include "IDataStream.h"
#include "IMsgHandler.h"
#include "IEventLoop.h"
#include "DataStreamMuxDemux.h"
#include <vector>
#include <stdint.h>

class ConnectDataStream;

class FPGAMgrServer : public virtual IMsgHandler, public virtual IEventLoop {
	friend class ConnectDataStream;
public:
	FPGAMgrServer();

	virtual ~FPGAMgrServer();

	void setBackend(IFPGABackend *backend);

	void addDataStream(IDataStream *ds);

	int start_server(int port);

	int run();

	int32_t recv_and_dispatch(int32_t timeout_ms=-1);

	bool message(uint8_t ep, const FPGAMgrMsg &msg);

private:

	void close_sideband_streams();

	void open_sideband_streams();

	void init_wait_connect();

	void handle_connect();

private:
	ConnectDataStream				*m_connect_stream;
	IFPGABackend					*m_backend;
	DataStreamMuxDemux				m_muxdemux;
	std::vector<IDataStream *>		m_data_streams;
	bool							m_sideband_open;
	int32_t							m_srv_sock;
	uint16_t						m_srv_port;

	bool							m_shutdown_req;
	bool							m_connected;

	uint8_t							*m_program_data;
	uint32_t						m_program_data_idx;

	IDataStream						*m_client_stream;
	IMsgHandler						*m_msg_handler;

};

#endif /* SRC_SERVER_FPGAMGRSERVER_H_ */
