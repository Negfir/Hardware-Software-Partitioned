#include "systemc.h"
#include "interface.h"
//#include "bus.h"
#include <fstream>
#define MEM_SIZE 36


class Memory: public sc_module
{
public:
    unsigned in_address;
    unsigned in_option;
    unsigned in_length;
    
    unsigned a[MEM_SIZE];
    unsigned b[MEM_SIZE];
    unsigned c[MEM_SIZE];

    //sc_port<bus_minion_if> MEM_port;


    Memory(sc_module_name nm){

        unsigned int a[MEM_SIZE]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9};
        unsigned int b[MEM_SIZE]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9};
        
        // for (int i=0;i<MEM_SIZE;i++){
        //     a[i] = a_init[i];
        //     b[i] = b_init[i];
        //     c[i] =0;
        // }

    }



      //SC_HAS_PROCESS(Memory);

    void memoryFunction()
    {
    while(1){

        //MEM_port->Listen(in_address, in_option, in_length);

      }

    }




  };
