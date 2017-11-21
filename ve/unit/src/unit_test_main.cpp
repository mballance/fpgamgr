/*
 * unit_test_main.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include <unistd.h>

static std::string		prv_build_dir;

const std::string &get_build_dir() {
	return prv_build_dir;
}

int main(int argc, char **argv) {
	int status;

	prv_build_dir = getenv("BUILD_DIR_A");

	testing::InitGoogleTest(&argc, argv);

	status = RUN_ALL_TESTS();

	return 0;
}



