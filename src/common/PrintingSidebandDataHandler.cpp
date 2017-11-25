/*
 * PrintingSidebandDataHandler.cpp
 *
 *  Created on: Nov 25, 2017
 *      Author: ballance
 */

#include "PrintingSidebandDataHandler.h"
#include <stdio.h>
#include <string.h>

PrintingSidebandDataHandler::PrintingSidebandDataHandler() : m_buf_idx(0) {
	// TODO Auto-generated constructor stub

}

PrintingSidebandDataHandler::~PrintingSidebandDataHandler() {
	// TODO Auto-generated destructor stub
}

void PrintingSidebandDataHandler::init(IDataStream *stream) {

}

void PrintingSidebandDataHandler::write(const void *data, size_t sz) {
	if (sz > 4096) {
		fprintf(stdout, "Error: over-size write packet (%d)\n", sz);
	} else {
		memcpy(m_buf, data, sz);
		m_buf[sz] = 0;
		fprintf(stdout, "Receive Data:\n%s\n", m_buf);
	}
}
