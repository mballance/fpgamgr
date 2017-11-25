/*
 * sideband_stream_smoketest.cpp
 *
 *  Created on: Nov 23, 2017
 *      Author: ballance
 */
#include "gtest/gtest.h"
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>
#include "FPGAMgrServer.h"
#include "FPGAMgrClient.h"
#include "PipeDataProducer.h"
#include "PollingDataReceiver.h"

static void *server_thread(void *ud) {
	FPGAMgrServer *srv = static_cast<FPGAMgrServer *>(ud);
	struct timeval tv;
	uint32_t state = 0;
	uint64_t start, curr;

	gettimeofday(&tv, 0);
	start = (uint64_t)(tv.tv_sec) * 1000 +
	    (uint64_t)(tv.tv_usec) / 1000;
	curr = start;

	PipeDataProducer *producer = new PipeDataProducer();
	srv->addDataStream(producer);

	fprintf(stdout, "--> server_thread::run\n");
	uint32_t cnt=0, valid_cnt=0;
	for (int32_t i=0; i<10; i++) {
		// Wait for 100ms
		do {
			srv->recv_and_dispatch(
					((curr-start)<100)?100-(curr-start):0);
			gettimeofday(&tv, 0);
			curr = (uint64_t)(tv.tv_sec) * 1000 +
					(uint64_t)(tv.tv_usec) / 1000;
		} while ((curr-start) < 100);

		// Send a message
		if (producer->get_fd() != -1) {
			fprintf(stdout, "Send message\n");
			fflush(stdout);
			producer->do_write("Hello\n", sizeof("Hello\n")+1);
		} else {
			// Try again
			fprintf(stdout, "producer::get_fd() == -1\n");
			i--;
		}

		// Update current time
		gettimeofday(&tv, 0);
		start = (uint64_t)(tv.tv_sec) * 1000 +
				(uint64_t)(tv.tv_usec) / 1000;
		curr = start;
	}

	// Just run the server after this
	// Must process shutdown, etc
	srv->run();

	fprintf(stdout, "<-- server_thread::run\n");

	return 0;
}

TEST(sideband_stream,smoke) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	uint16_t port = srv->start_server(0);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	FPGAMgrClient *client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);

	PollingDataReceiver *recv = new PollingDataReceiver(client);
	client->set_sideband_handler(1, recv);

	client->enable_sideband();

	for (int32_t i=0; i<10; i++) {
		uint8_t tmp[1024];
		size_t ret = recv->read(tmp, 1024);

		fprintf(stdout, "i=%d ret=%d \n", i, ret);
		if (ret > 0) {
			fprintf(stdout, "MSG=%s\n", tmp);
		}
	}

	client->disable_sideband();

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


