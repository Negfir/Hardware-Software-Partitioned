
#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include "bus.h"
#include "software.h"
#include <fstream>



int sc_main(int argc, char* argv[]) {
  Software SW("SW");
  //char* file = (char *)"mem_init.txt";
  Memory mem("mem");
  Bus bus("bus");
  SW.SW_port(bus);
  mem.MEM_port(bus);
  //SW.softwareFunction();
  sc_start();

  return(0);
}

