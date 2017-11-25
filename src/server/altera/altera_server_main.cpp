#include <stdio.h>
#include <stdint.h>
#include "FPGAMgrServer.h"
#include "CycloneVBackend.h"
#include "CharDevDataStream.h"

int main(int argc, char **argv) {
  uint16_t port = 6000;

  CycloneVBackend *backend = new CycloneVBackend();
  CharDevDataStream *ttyS1 = new CharDevDataStream("/dev/ttyS1");
 
  backend->setFpgaDev("/dev/fpga0");

  FPGAMgrServer *server = new FPGAMgrServer();
  server->setBackend(backend);
  
  server->addDataStream(ttyS1);

  server->start_server(port);

  server->run();


  return 0;
}


