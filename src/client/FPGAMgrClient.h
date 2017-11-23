/*
 * FPGAMgrClient.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_CLIENT_FPGAMGRCLIENT_H_
#define SRC_CLIENT_FPGAMGRCLIENT_H_
#include <string>
#include <stdint.h>
#include <vector>
#include "DataStreamMuxDemux.h"
#include "IMsgHandler.h"

class FPGAMgrClient : public virtual IMsgHandler {
public:
	FPGAMgrClient();

	virtual ~FPGAMgrClient();

	int connect(const std::string &host, uint16_t port);

	int32_t recv_and_dispatch(int32_t timeout_ms=-1);

	int program(const std::string &path);

	int shutdown_server();

	int close();

	bool message(uint8_t ep, const FPGAMgrMsg &msg);


private:

	bool recv(FPGAMgrMsg &msg);

private:
	DataStreamMuxDemux		m_muxdemux;
	bool					m_have_msg;
	FPGAMgrMsg				m_msg;
	IMsgHandler				*m_send;
	IDataStream				*m_stream;
	IDataHandler			*m_handler;

};

#endif /* SRC_CLIENT_FPGAMGRCLIENT_H_ */
