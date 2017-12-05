/*
 * CharDevSidebandStream.cpp
 *
 *  Created on: Dec 5, 2017
 *      Author: ballance
 */

#include "CharDevSidebandStream.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

CharDevSidebandStream::CharDevSidebandStream(
		const std::string			&path,
		IDataStream::StreamType		type) {
	m_fd = -1;
	m_path = path;
	m_type = type;
}

CharDevSidebandStream::~CharDevSidebandStream() {
	// TODO Auto-generated destructor stub
}


IDataStream::StreamType CharDevSidebandStream::get_type() {
	return m_type;
}

int CharDevSidebandStream::open() {
	if (m_fd == -1) {
		m_fd = ::open(m_path.c_str(), O_RDWR);
		fprintf(stdout, "Note: open %s (%d)\n", m_path.c_str(), m_fd);
	} else {
		fprintf(stdout, "Error: attempted double-open of stream device %s\n",
				m_path.c_str());
	}

	return m_fd;
}

int CharDevSidebandStream::close() {
	if (m_fd != -1) {
		::close(m_fd);
	}

	m_fd = -1;

	return 0;
}

size_t CharDevSidebandStream::read(void *data, size_t size) {
	return ::read(m_fd, data, size);
}

size_t CharDevSidebandStream::write(void *data, size_t size) {
	return ::write(m_fd, data, size);
}

bool CharDevSidebandStream::message(uint8_t ep, const FPGAMgrMsg &msg) {
	const uint8_t *data = msg.data();
	uint32_t idx = 0;

	while (idx < msg.size()) {
		uint32_t ret = ::write(m_fd, &data[idx], (msg.size()-idx));

		if (!ret) {
			break;
		}
		idx += ret;
	}

	return (idx == msg.size());
}

int CharDevSidebandStream::get_fd() {
	return m_fd;
}

