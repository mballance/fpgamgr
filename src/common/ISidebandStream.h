/*
 * ISidebandStream.h
 *
 *  Created on: Dec 5, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_ISIDEBANDSTREAM_H_
#define SRC_COMMON_ISIDEBANDSTREAM_H_
#include "IDataStream.h"
#include "IMsgHandler.h"

class ISidebandStream : public virtual IDataStream, public virtual IMsgHandler {
public:

	virtual ~ISidebandStream() { }

};




#endif /* SRC_COMMON_ISIDEBANDSTREAM_H_ */
