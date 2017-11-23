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
#include "DataStreamMuxDemux.h"
#include <vector>
#include <stdint.h>

class FPGAMgrServer : public IMsgHandler {
public:
	FPGAMgrServer();

	virtual ~FPGAMgrServer();

	void setBackend(IFPGABackend *backend);

	void addDataStream(IDataStream *ds);

	int start_server(int port);

	int run();

	bool message(uint8_t ep, const FPGAMgrMsg &msg);

private:

	bool recv(FPGAMgrMsg &msg);

private:
	IFPGABackend					*m_backend;
	DataStreamMuxDemux				m_muxdemux;
	std::vector<IDataStream *>		m_data_streams;
	int32_t							m_srv_sock;
	uint16_t						m_srv_port;
	bool							m_have_msg;
	FPGAMgrMsg						m_msg;

};

#endif /* SRC_SERVER_FPGAMGRSERVER_H_ */
