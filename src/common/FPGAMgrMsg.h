/*
 * FPGAMgrMsg.h
 *
 *  Created on: Nov 20, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_FPGAMGRMSG_H_
#define SRC_COMMON_FPGAMGRMSG_H_
#include <stdint.h>

class FPGAMgrMsg {
public:
	FPGAMgrMsg();

	FPGAMgrMsg(const FPGAMgrMsg &other);

	virtual ~FPGAMgrMsg();

	void operator = (const FPGAMgrMsg &rhs);

	void reset_get();

	void reset_put();

	uint8_t get8();

	uint8_t get8(uint32_t idx);

	void put8(uint8_t v);

	void put8(uint8_t v, uint32_t idx);

	uint32_t get32();

	uint32_t get32(uint32_t idx);

	void put32(uint32_t v);

	void put32(uint32_t v, uint32_t idx);

	void write(const void *p, uint32_t sz);

	void read(void *p, uint32_t sz);

	uint32_t size() const { return m_put_idx; }

	uint8_t *data() const { return m_data; }

	void dump();

private:

	uint8_t				*m_data;
	uint32_t			m_get_idx;
	uint32_t			m_put_idx;
	uint32_t			m_max;

};

#endif /* SRC_COMMON_FPGAMGRMSG_H_ */
