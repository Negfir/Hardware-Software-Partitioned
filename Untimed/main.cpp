
#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include <fstream>




int sc_main(int argc, char* argv[]) {
  sad sadModule("sadModule");
  //char* file = (char *)"mem_init.txt";
  memory mem("mem", (char *)argv[1]);

  sadModule.MEM(mem);
  sadModule.sadFunction();

  return(0);
}

