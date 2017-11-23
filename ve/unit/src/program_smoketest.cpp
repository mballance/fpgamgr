/*
 * program_smoketest.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */

#include "gtest/gtest.h"
#include <stdio.h>
#include <stdint.h>
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

TEST(program,smoke) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	TestBackend *backend = new TestBackend();
	srv->setBackend(backend);
	uint16_t port = srv->start_server(0);

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

	FPGAMgrClient *client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);

	ASSERT_TRUE(client->program("fpga_data.bin") == 0);

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

	ASSERT_TRUE(client->shutdown_server() == 0);

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

TEST(program,medium) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	TestBackend *backend = new TestBackend();
	srv->setBackend(backend);
	uint16_t port = srv->start_server(0);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	/**
	 * Create a random data file
	 */
	FILE *fp = fopen("fpga_data.bin", "wb");
	for (int i=0; i<(512*1024); i++) {
		int v = rand();
		fputc(v, fp);
	}
	fclose(fp);

	FPGAMgrClient *client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);

	ASSERT_TRUE(client->program("fpga_data.bin") == 0);

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

	ASSERT_TRUE(client->shutdown_server() == 0);

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

TEST(program,large) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	TestBackend *backend = new TestBackend();
	srv->setBackend(backend);
	uint16_t port = srv->start_server(0);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	/**
	 * Create a random data file
	 */
	FILE *fp = fopen("fpga_data.bin", "wb");
	for (int i=0; i<7000384; i++) {
		int v = rand();
		fputc(v, fp);
	}
	fclose(fp);

	FPGAMgrClient *client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);

	ASSERT_TRUE(client->program("fpga_data.bin") == 0);

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

	ASSERT_TRUE(client->shutdown_server() == 0);

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
