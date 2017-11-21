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

	bool recv(FPGAMgrMsg &msg);

	bool send(const FPGAMgrMsg &msg);

private:
	int get_c();

	bool read(FPGAMgrMsg &msg, uint32_t sz);

private:
	int					m_fd;
	uint8_t				m_buf[4096];
	uint32_t			m_buf_idx;
	uint32_t			m_buf_sz;
};

#endif /* SRC_COMMON_FPGAMGRMSGSTREAM_H_ */
