/*
 * CharDevDataStream.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "CharDevDataStream.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

CharDevDataStream::CharDevDataStream() {
	// TODO Auto-generated constructor stub

}

CharDevDataStream::~CharDevDataStream() {
	// TODO Auto-generated destructor stub
}

int CharDevDataStream::open(const char *path) {
	m_fd = ::open(path, O_RDWR);

	return (m_fd != -1);
}

size_t CharDevDataStream::read(void *data, size_t size) {
	return ::read(m_fd, data, size);
}

size_t CharDevDataStream::write(void *data, size_t size) {
	return ::write(m_fd, data, size);
}

int CharDevDataStream::get_fd() {
	return m_fd;
}
