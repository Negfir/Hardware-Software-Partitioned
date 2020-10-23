#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include <fstream>

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