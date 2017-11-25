/*
 * PollingDataReceiver.h
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#ifndef VE_UNIT_SRC_POLLINGDATARECEIVER_H_
#define VE_UNIT_SRC_POLLINGDATARECEIVER_H_
#include "IDataHandler.h"
#include "IEventLoop.h"

class PollingDataReceiver : public IDataHandler {
public:
	PollingDataReceiver(IEventLoop *ev_loop);

	virtual ~PollingDataReceiver();

	size_t read(void *p, uint32_t sz);

	virtual void write(const void *p, size_t sz);

	virtual void init(IDataStream *stream);

private:
	uint8_t				*m_cached_data;
	uint32_t			m_cached_data_sz;
	uint32_t			m_cached_data_max;
	IEventLoop			*m_ev_loop;
};

#endif /* VE_UNIT_SRC_POLLINGDATARECEIVER_H_ */
