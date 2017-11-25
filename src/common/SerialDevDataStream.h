/*
 * SerialDevDataStream.h
 *
 *  Created on: Nov 25, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_SERIALDEVDATASTREAM_H_
#define SRC_COMMON_SERIALDEVDATASTREAM_H_
#include "CharDevDataStream.h"

class SerialDevDataStream: public CharDevDataStream {
public:

	SerialDevDataStream(const std::string &dev);

	virtual ~SerialDevDataStream();

	virtual int open();
};

#endif /* SRC_COMMON_SERIALDEVDATASTREAM_H_ */
