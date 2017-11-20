/*
 * UUEncDec.h
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#ifndef SRC_COMMON_UUENCDEC_H_
#define SRC_COMMON_UUENCDEC_H_
#include <string>
#include <sys/types.h>

class UUEncDec {
public:
	UUEncDec();

	virtual ~UUEncDec();

	static std::string encode(
			const uint8_t		*data,
			size_t				sz,
			bool				escape=false);

	static int decode(
			const std::string	&data,
			uint8_t				*result,
			bool				is_escaped=false);

private:
	static uint8_t get(
			const std::string	&data,
			size_t				&idx,
			size_t				end,
			bool				is_escaped);
};

#endif /* SRC_COMMON_UUENCDEC_H_ */
