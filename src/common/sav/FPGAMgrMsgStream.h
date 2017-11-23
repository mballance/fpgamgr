/*
 * FPGAMgrMsgStream.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_FPGAMGRMSGSTREAM_H_
#define SRC_COMMON_FPGAMGRMSGSTREAM_H_
#include <string>
#include <vector>
#include <stdint.h>
#include "FPGAMgrMsg.h"
#include "IDataStream.h"
#include "IDataHandler.h"
#include "IMsgHandler.h"

class FPGAMgrMsgStream {
public:
	FPGAMgrMsgStream(int fd);

	virtual ~FPGAMgrMsgStream();

	bool recv(FPGAMgrMsg &msg);

	bool send(const FPGAMgrMsg &msg, uint8_t ep=0);

	int recv_and_dispatch(int32_t timeout_ms=-1);

	void add_data_stream(
			IDataStream 	*stream,
			IDataHandler	*handler);

	void set_msg_handler(
			IMsgHandler 	*handler,
			uint8_t			ep);

private:
	int get_c();

	bool read(FPGAMgrMsg &msg, uint32_t sz);

	size_t recv(uint8_t *data, size_t sz);

private:
	enum recv_state_e {
		ST_WAIT_MARKER,
		ST_RECV_
	};

private:
	int							m_fd;
	uint8_t						m_buf[4096];
	uint32_t					m_buf_idx;
	uint32_t					m_buf_sz;
	std::vector<IDataStream *>	m_data_streams;
	std::vector<IDataHandler *> m_data_handlers;
	std::vector<IMsgHandler *>	m_msg_handlers;
	FPGAMgrMsg					m_msg;
};

#endif /* SRC_COMMON_FPGAMGRMSGSTREAM_H_ */
