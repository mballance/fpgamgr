/*
 * altera_sockit_server.cpp
 *
 *  Created on: Nov 18, 2017
 *      Author: ballance
 */
#include "FPGAMgrServer.h"
#include "CycloneVBackend.h"
#include "CharDevDataStream.h"

int main(int argc, char **argv) {
	FPGAMgrServer *server = new FPGAMgrServer();
	CycloneVBackend *backend = new CycloneVBackend();
	backend->setFpgaDev("/dev/fpga0");

	CharDevDataStream *ttyS1 = new CharDevDataStream();
	ttyS1->open("/dev/ttyS1");

	server->addDataStream(ttyS1);

	// For now,

}




