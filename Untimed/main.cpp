
#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include "bus.h"
#include "software.h"
#include "hardware.h"
#include <fstream>



int sc_main(int argc, char* argv[]) {
  Software SW("SW");
  //char* file = (char *)"mem_init.txt";
  Memory mem("mem");
  Hardware HW("HW");
  Bus bus("bus");
  SW.SW_port(bus);
  mem.MEM_port(bus);
  HW.HW_master_port(bus);
  HW.HW_minion_port(bus);
  //SW.softwareFunction();
  sc_start();
   //sc_start();

  return(0);
}

