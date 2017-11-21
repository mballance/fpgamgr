#include <stdio.h>
#include <stdint.h>
#include "FPGAMgrServer.h"
#include "CycloneVBackend.h"

int main(int argc, char **argv) {
  uint16_t port = 6000;

  CycloneVBackend *backend = new CycloneVBackend();
 
  backend->setFpgaDev("/dev/fpga0");

  FPGAMgrServer *server = new FPGAMgrServer();
  server->setBackend(backend);
  
  server->start_server(port);

  server->run();


  return 0;
}


