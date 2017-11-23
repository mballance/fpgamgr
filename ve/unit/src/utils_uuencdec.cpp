/*
 * utils_uuencdec.cpp
 *
 *  Created on: Nov 19, 2017
 *      Author: ballance
 */
#include "UUEncDec.h"
#include "gtest/gtest.h"

TEST(utils_uuencdec,smoke) {
	uint8_t *data = new uint8_t[400];

	for (int i=0; i<400; i++) {
		data[i] = i;
	}

	std::string enc = UUEncDec::encode(data, 400);

	int size = UUEncDec::decode(enc, 0, false);

	ASSERT_EQ(size, 400);

	uint8_t *data2 = new uint8_t[size];
	size = UUEncDec::decode(enc, data2, false);
	ASSERT_EQ(size, 400);

	for (int i=0; i<400; i++) {
		ASSERT_EQ(data[i], data2[i]);
	}

	enc = UUEncDec::encode(data, 400, true);

	fprintf(stdout, "enc (escaped):\n%s\n", enc.c_str());

	size = UUEncDec::decode(enc, 0, true);

	ASSERT_EQ(size, 400);

	data2 = new uint8_t[size];
	size = UUEncDec::decode(enc, data2, true);
	ASSERT_EQ(size, 400);

	for (int i=0; i<400; i++) {
		ASSERT_EQ(data[i], data2[i]);
	}


}

#ifdef UNDEFINED
TEST(utils_uuencdec,json) {
	uint8_t *data = new uint8_t[400];

	for (int i=0; i<400; i++) {
		data[i] = i;
	}

	std::string enc = UUEncDec::encode(data, 400, true);

	json req;
	req["data"] = enc;

	std::string req_s = req.dump();

	json req_m = json::parse(req_s);

	std::string data_s = req_m["data"].get<std::string>();;

	int size = UUEncDec::decode(data_s, 0, true);

	ASSERT_EQ(size, 400);

	uint8_t *data2 = new uint8_t[size];
	size = UUEncDec::decode(data_s, data2, true);
	ASSERT_EQ(size, 400);

	for (int i=0; i<400; i++) {
		ASSERT_EQ(data[i], data2[i]);
	}

}
#endif




