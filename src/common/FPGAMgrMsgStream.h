/*
 * FPGAMgrMsgStream.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_FPGAMGRMSGSTREAM_H_
#define SRC_COMMON_FPGAMGRMSGSTREAM_H_
#include <string>
#include <stdint.h>
#include "FPGAMgrMsg.h"

class FPGAMgrMsgStream {
public:
	FPGAMgrMsgStream(int fd);

	virtual ~FPGAMgrMsgStream();

	FPGAMgrMsg recv();

	void send(const std::string &msg);

	void send(const FPGAMgrMsg &msg);

private:
	int get_c();

	void read(FPGAMgrMsg &msg, uint32_t sz);

private:
	int					m_fd;
	uint8_t				m_buf[4096];
	uint32_t			m_buf_idx;
	uint32_t			m_buf_sz;
};

#endif /* SRC_COMMON_FPGAMGRMSGSTREAM_H_ */
