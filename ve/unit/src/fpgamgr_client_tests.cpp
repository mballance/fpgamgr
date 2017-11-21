/*
 * fpgamgr_client_tests.cpp
 *
 *  Created on: Nov 21, 2017
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include "unit_test_main.h"
#include <stdio.h>

#include <pthread.h>
#include "FPGAMgrServer.h"
#include "FPGAMgrClient.h"
#include "IFPGABackend.h"

static void *server_thread(void *ud) {
	FPGAMgrServer *srv = static_cast<FPGAMgrServer *>(ud);

	fprintf(stdout, "--> server_thread::run\n");
	srv->run();
	fprintf(stdout, "<-- server_thread::run\n");

	return 0;
}

class TestBackend : public IFPGABackend {
public:

	int program(void *data, size_t size) {
		FILE *fp = fopen("fpga_img.bin", "wb");

		fwrite(data, 1, size, fp);
		fclose(fp);

		return 0;
	}
};

TEST(fpgamgr_client,program) {
	std::string build_dir = get_build_dir();
	std::string fpgamgr_client = build_dir + "/fpgamgr_client";
	char port_s[64];

	FPGAMgrServer *srv = new FPGAMgrServer();
	TestBackend *backend = new TestBackend();
	srv->setBackend(backend);
	uint16_t port = srv->start_server(0);
	sprintf(port_s, "%d", port);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	/**
	 * Create a random data file
	 */
	FILE *fp = fopen("fpga_data.bin", "wb");
	for (int i=0; i<1000; i++) {
		int v = rand();
		fputc(v, fp);
	}
	fclose(fp);

	// First program
	std::string cmd = fpgamgr_client +
			" program -host localhost -port " + port_s +
			" fpga_data.bin";

	fprintf(stdout, "cmd=%s\n", cmd.c_str());
	system(cmd.c_str());

	// Compare data
	fp = fopen("fpga_data.bin", "rb");
	FILE *sfp = fopen("fpga_img.bin", "rb");

	ASSERT_TRUE(fp);
	ASSERT_TRUE(sfp);

	int fp_c, sfp_c;
	do {
		fp_c = fgetc(fp);
		sfp_c = fgetc(sfp);

		ASSERT_EQ(fp_c, sfp_c);
	} while (fp_c != -1 && sfp_c != -1);

	fclose(fp);
	fclose(sfp);

	cmd = fpgamgr_client +
			" shutdown -host localhost -port " + port_s;
	system(cmd.c_str());

	void *ret = 0;
	bool joined = false;
	for (int i=0; i<5; i++) {
		if (pthread_tryjoin_np(t, &ret) == 0) {
			joined = true;
			break;
		} else {
			::sleep(1);
		}
	}

	ASSERT_TRUE(joined);
}

