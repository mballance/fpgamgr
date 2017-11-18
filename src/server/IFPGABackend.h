/*
 * IFPGABackend.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_SERVER_IFPGABACKEND_H_
#define SRC_SERVER_IFPGABACKEND_H_
#include <sys/types.h>

class IFPGABackend {
public:

	virtual ~IFPGABackend() { }

	/**
	 * Program the device with the specified data
	 */
	virtual int program(void *data, size_t size) = 0;

};




#endif /* SRC_SERVER_IFPGABACKEND_H_ */
