/*
 * PrintingSidebandDataHandler.h
 *
 *  Created on: Nov 25, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_PRINTINGSIDEBANDDATAHANDLER_H_
#define SRC_COMMON_PRINTINGSIDEBANDDATAHANDLER_H_

#include "IDataHandler.h"
#include <stdint.h>

class PrintingSidebandDataHandler: public IDataHandler {
public:
	PrintingSidebandDataHandler();

	virtual ~PrintingSidebandDataHandler();

	virtual void init(IDataStream *stream);

	virtual void write(const void *data, size_t sz);

private:
	uint8_t				m_buf[4096];
	uint32_t			m_buf_idx;

};

#endif /* SRC_COMMON_PRINTINGSIDEBANDDATAHANDLER_H_ */
