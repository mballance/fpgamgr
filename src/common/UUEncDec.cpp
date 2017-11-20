/*
 * UUEncDec.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "UUEncDec.h"

UUEncDec::UUEncDec() {
	// TODO Auto-generated constructor stub

}

UUEncDec::~UUEncDec() {
	// TODO Auto-generated destructor stub
}

std::string UUEncDec::encode(
		const uint8_t		*data,
		size_t				sz,
		bool				escape) {
	std::string result;

	char line[128];
	size_t len;
	size_t idx=0;

	while (idx<sz) {
		len = ((sz-idx)>45)?45:(sz-idx);

		uint32_t lp = 0;
		line[lp++] = len+32; // length

		// Pack 3 bytes as 4 characters
		for (uint32_t i=0; i<len; i+=3) {
			uint32_t td = (((uint32_t)data[idx+i]) & 0xFF);

			if (i+1 < len) {
				td |= ((((uint32_t)data[idx+i+1]) & 0xFF) << 8);
			}

			if (i+2 < len) {
				td |= ((((uint32_t)data[idx+i+2]) & 0xFF) << 16);
			}

			// Now, break down into four characters
			for (uint32_t j=0; j<4; j++) {
				line[lp++] = ((td >> 6*j) & 0x3F) + 32;

				if (escape && (line[lp-1] == '"' || line[lp-1] == '\\')) {
					line[lp] = line[lp-1];
					line[lp-1] = '\\';
					lp++;
				}
			}
		}

		line[lp] = 0;
		result.append(line);
		if (escape) {
			result.append("\\n");
		} else {
			result.append("\n");
		}

		idx += len;
	}

	return result;
}

int UUEncDec::decode(
		const std::string	&data,
		uint8_t				*result,
		bool				is_escaped) {
	char tmp[4];

	// Read a line at a time
	int sz = 0;
	size_t idx=0;
	uint32_t tmp_d;
	while (idx < data.size()) {
		uint8_t len = (data.at(idx++)-32); // get the actual length
		size_t end;

		if (is_escaped) {
			if ((end=data.find("\\n", idx)) == std::string::npos) {
				end = data.size();
			}
		} else {
			if ((end=data.find('\n', idx)) == std::string::npos) {
				end = data.size();
			}
		}

		for (uint32_t i=0; i<len; i+=3) {
			tmp[0] = get(data, idx, end, is_escaped);
			tmp[1] = get(data, idx, end, is_escaped);
			tmp[2] = get(data, idx, end, is_escaped);
			tmp[3] = get(data, idx, end, is_escaped);

			tmp_d = ((tmp[0] << 0) |
					(tmp[1] << 6)      |
					(tmp[2] << 12)     |
					(tmp[3] << 18));
			if (result) {
				result[sz++] = ((tmp_d >> 0) & 0xFF);
			} else {
				sz++;
			}
			if (i+1 < len) {
				if (result) {
					result[sz++] = ((tmp_d >> 8) & 0xFF);
				} else {
					sz++;
				}
				if (i+2 < len) {
					if (result) {
						result[sz++] = ((tmp_d >> 16) & 0xFF);
					} else {
						sz++;
					}
				}
			}
		}

		if (is_escaped) {
			idx = end+2;
		} else {
			idx = end+1;
		}
	}

	return sz;
}

uint8_t UUEncDec::get(
		const std::string		&data,
		size_t					&idx,
		size_t					end,
		bool					is_escaped) {
	if (idx >= end) {
		return 0;
	} else if (is_escaped) {
		if (data.at(idx) == '\\') {
			idx++;
		}
		return (data.at(idx++)-32);
	} else {
		return (data.at(idx++)-32);
	}
}

