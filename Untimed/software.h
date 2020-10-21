#include "systemc.h"
#include "interface.h"
#include <fstream>
#define SIZE 5

class Software: public sc_module
{
public:
    unsigned a[SIZE][SIZE];
    unsigned b[SIZE][SIZE];
    unsigned c[SIZE][SIZE];
    
    sc_port<bus_master_if> SW_port;
    unsigned result;
    
    Software(sc_module_name nm);
    
  
};
