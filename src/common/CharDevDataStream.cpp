/*
 * CharDevDataStream.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "../common/CharDevDataStream.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

CharDevDataStream::CharDevDataStream(const std::string &path) {
	m_path = path;
}

CharDevDataStream::~CharDevDataStream() {
	// TODO Auto-generated destructor stub
}

int CharDevDataStream::open() {
	m_fd = ::open(m_path.c_str(), O_RDWR);

	return (m_fd != -1)?0:-1;
}

int CharDevDataStream::close() {
	::close(m_fd);

	m_fd = -1;

	return 0;
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
