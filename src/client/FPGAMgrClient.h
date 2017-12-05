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
#include "FPGAMgrMsgDataHandler.h"
#include "IMsgHandler.h"
#include "IEventLoop.h"

class FPGAMgrClient :
		public virtual IMsgHandler, public virtual IEventLoop {
public:
	FPGAMgrClient();

	virtual ~FPGAMgrClient();

	void set_sideband_handler(uint8_t ep, IDataHandler *handler);

	int connect(const std::string &host, uint16_t port);

	int32_t recv_and_dispatch(int32_t timeout_ms=-1);

	int program(const std::string &path);

	int enable_sideband();

	int disable_sideband();

	int shutdown_server();

	int close();

	bool message(uint8_t ep, const FPGAMgrMsg &msg);

	size_t write(uint8_t ep, void *data, size_t sz);

private:

	bool recv(FPGAMgrMsg &msg);

private:
	DataStreamMuxDemux			m_muxdemux;
	bool						m_have_msg;
	FPGAMgrMsg					m_msg;
	IMsgHandler					*m_send;
	IDataStream					*m_stream;
	FPGAMgrMsgDataHandler		*m_handler;
	std::vector<IDataHandler *>  m_sideband_handlers;

};

#endif /* SRC_CLIENT_FPGAMGRCLIENT_H_ */
