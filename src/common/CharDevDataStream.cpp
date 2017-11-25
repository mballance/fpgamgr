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

CharDevDataStream::CharDevDataStream(const std::string &path, IDataStream::StreamType type) {
	m_fd = -1;
	m_path = path;
	m_type = type;
}

CharDevDataStream::~CharDevDataStream() {
	// TODO Auto-generated destructor stub
}

IDataStream::StreamType CharDevDataStream::get_type() {
	return m_type;
}

int CharDevDataStream::open() {
	if (m_fd == -1) {
		m_fd = ::open(m_path.c_str(), O_RDWR);
	} else {
		fprintf(stdout, "Error: attempted double-open of stream device %s\n",
				m_path.c_str());
	}

	return (m_fd != -1)?0:-1;
}

int CharDevDataStream::close() {
	if (m_fd != -1) {
		::close(m_fd);
	}

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
