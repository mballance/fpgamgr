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
#include <vector>
#include <stdint.h>

class FPGAMgrServer {
public:
	FPGAMgrServer();

	virtual ~FPGAMgrServer();

	void setBackend(IFPGABackend *backend);

	void addDataStream(IDataStream *ds);

	int start_server(int port);

	int run();

private:
	IFPGABackend					*m_backend;
	std::vector<IDataStream *>		m_data_streams;
	int32_t							m_srv_sock;
	uint16_t						m_srv_port;

};

#endif /* SRC_SERVER_FPGAMGRSERVER_H_ */
