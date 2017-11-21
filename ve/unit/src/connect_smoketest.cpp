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

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	FPGAMgrClient *client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);

	ASSERT_TRUE(client->shutdown_server() == 0);

	client->close();

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

TEST(connect,disconnect) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	uint16_t port = srv->start_server(0);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);

	FPGAMgrClient *client = new FPGAMgrClient();

	for (int i=0; i<10; i++) {
		ASSERT_TRUE(client->connect("localhost", port) == 0);
		ASSERT_TRUE(client->close() == 0);
	}

	ASSERT_TRUE(client->connect("localhost", port) == 0);
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

TEST(connect,hangup) {
	FPGAMgrServer *srv = new FPGAMgrServer();
	uint16_t port = srv->start_server(0);

	pthread_t t;
	pthread_create(&t, 0, &server_thread, srv);


	FPGAMgrClient *client;
	for (int i=0; i<10; i++) {
		client = new FPGAMgrClient();
		ASSERT_TRUE(client->connect("localhost", port) == 0);
		delete client;
	}

	client = new FPGAMgrClient();
	ASSERT_TRUE(client->connect("localhost", port) == 0);
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
