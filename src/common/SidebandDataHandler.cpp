/*
 * SidebandDataHandler.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#include "SidebandDataHandler.h"

SidebandDataHandler::SidebandDataHandler(
		uint8_t				ep,
		IMsgHandler			*msg_handler
		) : m_ep(ep), m_stream(0), m_msg_handler(msg_handler) {

}

SidebandDataHandler::~SidebandDataHandler() {
	// TODO Auto-generated destructor stub
}

void SidebandDataHandler::init(IDataStream *stream) {
	m_stream = stream;
}

void SidebandDataHandler::write(const void *p, size_t sz) {
	// TODO: deal with small data packets

	FPGAMgrMsg msg;
	msg.write(p, sz);

	// Send the message to the other side
	m_msg_handler->message(m_ep, msg);
}

bool SidebandDataHandler::message(uint8_t ep, const FPGAMgrMsg &msg) {

	// Send message data to stream
	uint8_t *data = msg.data();
	uint32_t sz = msg.size();
	uint32_t i=0;

	while (i<sz) {
		size_t ret = m_stream->write(&data[i], (sz-i));
		if (ret == 0) {
			break;
		}
		i += ret;
	}

	return (i==sz);
}
