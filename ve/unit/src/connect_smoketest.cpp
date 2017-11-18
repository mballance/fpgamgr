/*
 * connect_smoketest.cpp
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

static void *server_thread(void *ud) {
	FPGAMgrServer *srv = static_cast<FPGAMgrServer *>(ud);

	fprintf(stdout, "--> server_thread::run\n");
	srv->run();
	fprintf(stdout, "<-- server_thread::run\n");

	return 0;
}

TEST(connect,smoke) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	uint16_t port = srv->start_server(0);
	fprintf(stdout, "port=%d\n", port);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	FPGAMgrClient *client = new FPGAMgrClient();
	fprintf(stdout, "--> connect\n");
	client->connect("localhost", port);
	fprintf(stdout, "<-- connect\n");

	client->shutdown_server();

	client->close();
}



