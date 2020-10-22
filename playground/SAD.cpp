
#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include <fstream>

#define NUM_BLOCKS 64
#define BLOCK_SIZE 64
#define INPUT1_ADDR 0
#define INPUT2_ADDR 16384
#define SAD_OUTPUT_ADDR 32768
//#define SAD_OUTPUT_ADDR 32768

SC_MODULE(sad) {
  sc_port<bus_master_if> SW_port;

  void sadFunction() {
          
    //SW_port->Request(1, 1, 3, 8);
    SW_port->WriteData(3);
  }


  SC_CTOR(sad)       {
    SC_METHOD(sadFunction);   
  }


};



int sc_main(int argc, char* argv[]) {
  sad sadModule("sadModule");
  //char* file = (char *)"mem_init.txt";
  memory mem("mem", (char *)argv[1]);

  sadModule.SW_port(mem);
  sadModule.sadFunction();

  return(0);
}

