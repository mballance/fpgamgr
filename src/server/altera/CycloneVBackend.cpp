/*
 * CycloneVBackend.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "CycloneVBackend.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

CycloneVBackend::CycloneVBackend() {
	// TODO Auto-generated constructor stub

}

CycloneVBackend::~CycloneVBackend() {
	// TODO Auto-generated destructor stub
}

void CycloneVBackend::setFpgaDev(const std::string &path) {
	m_fpga_dev = path;
}

int CycloneVBackend::program(void *data, size_t size) {
	const uint8_t *data8 = (uint8_t *)data;
	int fd = ::open(m_fpga_dev.c_str(), O_WRONLY);
	uint32_t idx=0;

	while (size) {
		uint32_t sz = (size>128)?128:size;
		size_t ret = ::write(fd, &data8[idx], sz);

		if (ret != sz) {
			fprintf(stdout, "Error: ret=%d sz=%d\n", ret, sz);
		}
		idx += sz;
		size -= sz;
	}

	::close(fd);

	return 0;
}
