
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include "FPGAMgrClient.h"
#include "PrintingSidebandDataHandler.h"

static void print_help() {

}

static uint16_t port = 6000;
static std::string host = "localhost";

int parse_common_options(std::vector<std::string> &args) {
	for (int i=0; i<args.size(); i++) {
		if (args.at(i) == "-port") {
			port = ::strtoul(args.at(i+1).c_str(), 0, 0);
			args.erase(args.begin()+i+1);
			args.erase(args.begin()+i);
			i--;
		} else if (args.at(i) == "-host") {
			host = args.at(i+1).c_str();
			args.erase(args.begin()+i+1);
			args.erase(args.begin()+i);
			i--;
		} // Ignore unknown options
	}

	return 0;
}

int cmd_program(std::vector<std::string> &args) {
	FPGAMgrClient client;
	std::string file;

	for (int i=1; i<args.size(); i++) {
		if (args.at(i).at(0) == '-') {
		} else {
			// Command
			if (file == "") {
				file = args.at(i);
			} else {
				fprintf(stdout, "Error: unknown \"program\" argument: %s\n",
						args.at(i).c_str());
				print_help();
				return 1;
			}
		}
	}

	if (file == "") {
		fprintf(stdout, "Error: no data file specified\n");
		return 1;
	}

	if (client.connect(host, port) != 0) {
		fprintf(stdout, "Error: failed to connect\n");
		return 1;
	}

	if (client.program(file) != 0) {
		client.close();
		return 1;
	}

	client.close();

	return 0;
}

int cmd_shutdown(std::vector<std::string> &args) {
	FPGAMgrClient client;

	if (client.connect(host, port) != 0) {
		fprintf(stdout, "Error: failed to connect\n");
		return 1;
	}

	return client.shutdown_server();
}

int cmd_dump_sideband(std::vector<std::string> &args) {
	FPGAMgrClient client;

	if (client.connect(host, port) != 0) {
		fprintf(stdout, "Error: failed to connect\n");
		return 1;
	}

	// Attach a printing monitor
	PrintingSidebandDataHandler *handler = new PrintingSidebandDataHandler();
	client.set_sideband_handler(1, handler);

	client.enable_sideband();

	while (client.recv_and_dispatch(-1) >= 0) {
		;
	}

	return 0;
}

int main(int argc, char **argv) {
	int ret;

	// First detect a help request
	for (int i=1; i<argc; i++) {
		std::string arg = argv[i];
		if (arg == "-help" || arg == "--help" ||
				arg == "-h" || arg == "--h" ||
				arg == "-?") {
			print_help();
			return 0;
		}
	}

	if (argc < 2) {
		fprintf(stdout, "Error: missing subcommand argument\n");
		print_help();
		return 1;
	}

	std::vector<std::string> args;

	for (int i=1; i<argc; i++) {
		args.push_back(argv[i]);
	}

	if (parse_common_options(args) != 0) {
		return 1;
	}

	std::string cmd = args.at(0);

	if (cmd == "program") {
		ret = cmd_program(args);
	} else if (cmd == "shutdown") {
		ret = cmd_shutdown(args);
	} else if (cmd == "dump_sideband") {
		ret = cmd_dump_sideband(args);
	} else {
		fprintf(stdout, "Error: unknown subcommand %s\n", cmd.c_str());
		print_help();
		return 1;
	}

	return ret;
}

