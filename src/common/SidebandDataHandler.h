/*
 * SidebandDataHandler.h
 *
 * Connects sideband data via messages
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_SIDEBANDDATAHANDLER_H_
#define SRC_COMMON_SIDEBANDDATAHANDLER_H_

#include "IDataHandler.h"
#include "IMsgHandler.h"
#include "IDataStream.h"

class SidebandDataHandler: public IDataHandler, public IMsgHandler {
public:
	SidebandDataHandler(
			uint8_t			ep,
			IMsgHandler		*msg_handler);

	virtual ~SidebandDataHandler();

	virtual void init(IDataStream *stream);

	// Data-handler method
	virtual void write(const void *p, size_t sz);

	// Message-handler method
	virtual bool message(uint8_t ep, const FPGAMgrMsg &msg);

private:

	uint8_t					m_ep;
	IDataStream				*m_stream;
	IMsgHandler				*m_msg_handler;
};

#endif /* SRC_COMMON_SIDEBANDDATAHANDLER_H_ */
