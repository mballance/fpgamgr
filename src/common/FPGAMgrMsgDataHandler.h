/*
 * FPGAMgrMsgDataHandler.h
 *
 *  Created on: Nov 22, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_FPGAMGRMSGDATAHANDLER_H_
#define SRC_COMMON_FPGAMGRMSGDATAHANDLER_H_

#include "IDataHandler.h"
#include "IMsgHandler.h"
#include "IDataStream.h"
#include "FPGAMgrMsg.h"
#include <vector>

class FPGAMgrMsgDataHandler:
		public virtual IDataHandler, public virtual IMsgHandler {
public:
	FPGAMgrMsgDataHandler();

	virtual ~FPGAMgrMsgDataHandler();

	virtual void init(IDataStream *stream);

	virtual void write(void *p, size_t sz);

	void set_msg_handler(uint32_t id, IMsgHandler *handler);

	// Handler to send messages
	bool message(uint8_t ep, const FPGAMgrMsg &msg);

private:
	enum recv_state_e {
		RS_MARKER,
		RS_HEADER,
		RS_DATA
	};
private:
	recv_state_e				m_recv_state;
	uint8_t						m_hdr[5];
	uint32_t					m_hdr_idx;
	uint32_t					m_data_sz;
	uint32_t					m_ep;
	FPGAMgrMsg					m_msg;
	std::vector<IMsgHandler *>	m_msg_handlers;
	IDataStream					*m_stream;

};

#endif /* SRC_COMMON_FPGAMGRMSGDATAHANDLER_H_ */
