/*
 * fpga_test_main.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: ballance
 */
#include "FPGAMgrClient.h"
#include "PollingDataReceiver.h"
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
	std::string host;
	std::string img;
	uint16_t port = 0;
	char		tmp1[128], tmp2[128];

	for (int i=1; i<argc; i++) {
		if (argv[i][0] == '+') {
			if (!strncmp(argv[i], "+host=", strlen("+host="))) {
				host = &argv[i][strlen("+host=")];
			} else if (!strncmp(argv[i], "+port=", strlen("+port="))) {
				port = strtoul(&argv[i][strlen("+port=")], 0, 0);
			} else if (!strncmp(argv[i], "+img=", strlen("+img="))) {
				img = &argv[i][strlen("+img=")];
			} else {
				fprintf(stdout, "Error: unknown option %s\n", argv[i]);
				exit(1);
			}
		} else {
			fprintf(stdout, "Error: Unknown argument %s\n", argv[i]);
		}
	}

	if (host == "" || port == 0) {
		fprintf(stdout, "Error: host or port not specified\n");
		exit(1);
	}

	if (img == "") {
		fprintf(stdout, "Error: image not specified\n");
		exit(1);
	}

	FPGAMgrClient *client = new FPGAMgrClient();
	PollingDataReceiver *recv = new PollingDataReceiver(client);

	if (client->connect(host, port) != 0) {
		fprintf(stdout, "Error: connect failed\n");
		exit(1);
	}

	// Register a handler for sideband channel 1
	client->set_sideband_handler(1, recv);

	if (client->program(img) != 0) {
		fprintf(stdout, "Error: programming failed\n");
		exit(1);
	}

	// Connect the sideband channels on the server side
	if (client->enable_sideband() != 0) {
		fprintf(stdout, "Error: failed to enable sideband\n");
		exit(1);
	}

	uint32_t count=100;
	for (uint32_t i=1; i<=count; i++) {
		sprintf(tmp1, "Hello World %d", i);

		fprintf(stdout, "--> Send %s\n", tmp1);
		client->write(1, tmp1, strlen(tmp1)+1);
		fprintf(stdout, "<-- Send %s\n", tmp1);

		fprintf(stdout, "--> Recv\n");
		uint32_t sz = recv->read(tmp2, sizeof(tmp2));

		if (sz != strlen(tmp1)+1) {
			fprintf(stdout, "Error: incorrect length expect %d received %d\n",
					strlen(tmp1)+1, sz);
		} else {
			fprintf(stdout, "<-- Recv %s\n", tmp2);
		}
	}

	fprintf(stdout, "Done with %d round trips!\n", count);

	// Cleanly exit
	client->close();

	exit(0);
}



