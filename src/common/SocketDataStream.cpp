/*
 * SocketDataStream.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */

#include "SocketDataStream.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

SocketDataStream::SocketDataStream(int fd, IDataStream::StreamType type) :
	m_fd(fd), m_type(type) {

}

SocketDataStream::~SocketDataStream() {
	if (m_fd >= 0) {
		::close(m_fd);
	}
	// TODO Auto-generated destructor stub
}

IDataStream::StreamType SocketDataStream::get_type() {
	return m_type;
}

int SocketDataStream::open() {
	// NOP
	return (m_fd >= 0)?0:-1;
}

int SocketDataStream::close() {
	if (m_fd != -1) {
		::close(m_fd);
	}
	m_fd = -1;
	return 0;
}

size_t SocketDataStream::read(void *data, size_t sz) {
	return ::recv(m_fd, data, sz, 0);
}

size_t SocketDataStream::write(void *data, size_t sz) {
	return ::send(m_fd, data, sz, 0);
}

int SocketDataStream::get_fd() {
	return m_fd;
}
