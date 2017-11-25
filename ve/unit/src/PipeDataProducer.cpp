/*
 * PipeDataProducer.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#include "PipeDataProducer.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

PipeDataProducer::PipeDataProducer() : m_fds{-1,-1} {
	// TODO Auto-generated constructor stub

}

PipeDataProducer::~PipeDataProducer() {
	// TODO Auto-generated destructor stub
}

int PipeDataProducer::open() {
	::pipe(m_fds);
	fprintf(stdout, "PipeDataProducer::open %d, %d\n",
			m_fds[0], m_fds[1]);

	return 0;
}

int PipeDataProducer::close() {
	if (m_fds[0] >= 0) {
		::close(m_fds[0]);
		::close(m_fds[1]);
		m_fds[0] = -1;
		m_fds[1] = -1;
	}
	return -1;
}

size_t PipeDataProducer::read(void *data, size_t bytes) {
	return ::read(m_fds[0], data, bytes);
}

size_t PipeDataProducer::write(void *data, size_t bytes) {
	// TODO:
	return 0;
}

size_t PipeDataProducer::do_write(const void *data, size_t bytes) {
	return ::write(m_fds[1], data, bytes);
}

int PipeDataProducer::get_fd() {
	return m_fds[0];
}
