/*
 * unit_test_main.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */
#include "gtest/gtest.h"

int main(int argc, char **argv) {
	int status;

	testing::InitGoogleTest(&argc, argv);

	status = RUN_ALL_TESTS();

	return 0;
}



