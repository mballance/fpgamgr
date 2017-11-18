/*
 * CycloneVBackend.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_SERVER_ALTERA_CYCLONEVBACKEND_H_
#define SRC_SERVER_ALTERA_CYCLONEVBACKEND_H_
#include "IFPGABackend.h"
#include <sys/types.h>
#include <string>

class CycloneVBackend: public IFPGABackend {
public:
	CycloneVBackend();

	virtual ~CycloneVBackend();

	void setFpgaDev(const std::string &path);

	virtual int program(void *data, size_t size);

private:
	std::string				m_fpga_dev;
};

#endif /* SRC_SERVER_ALTERA_CYCLONEVBACKEND_H_ */
