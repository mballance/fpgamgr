/*
 * SerialDevDataStream.cpp
 *
 *  Created on: Nov 25, 2017
 *      Author: ballance
 */

#include "SerialDevDataStream.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>

SerialDevDataStream::SerialDevDataStream(const std::string &path) :
	CharDevDataStream(path) {
	// TODO Auto-generated constructor stub

}

SerialDevDataStream::~SerialDevDataStream() {
	// TODO Auto-generated destructor stub
}

int SerialDevDataStream::open() {
	int fd = CharDevDataStream::open();

	if (fd != -1) {
		// Configure the descriptor
		struct termios tio;

		memset(&tio, 0, sizeof(tio));
		tio.c_iflag = 0;
		tio.c_oflag = 0;
		tio.c_cflag=CS8|CREAD|CLOCAL;
		tio.c_lflag = 0;
		tio.c_cc[VMIN] = 1;
		tio.c_cc[VTIME]=5;

		cfsetospeed(&tio, B115200);
		cfsetispeed(&tio, B115200);

		if (tcsetattr(fd, TCSANOW, &tio) != 0) {
			fprintf(stdout, "Error: tcsetattr failed\n");
		}
	}

	return fd;
}

