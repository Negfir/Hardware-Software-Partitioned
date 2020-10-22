#include "systemc.h"
#include "interface.h"
//#include "bus.h"
#include <fstream>
#define SIZE 6
SC_MODULE (Software)
//class Software: public sc_module
{
    sc_port<simple_mem_if> SW_port;

    unsigned a[SIZE][SIZE];
    unsigned b[SIZE][SIZE];
    unsigned c[SIZE][SIZE];
    
    
    unsigned result;
    


  void softwareFunction() 
  {
    int n;
    int i,j,k;

    unsigned int data=0;

    // for (n=0; n<5; n++) // Total Cycles: 8186006, Execs: 1, Iters: 1000
    // {
    //   for(i=0; i<SIZE; i++) // Total Cycles: 579000, Execs: 1000, Iters: 5
    //     for(j=0; j<SIZE; j++) // Total Cycles: 520000, Execs: 5000, Iters: 5
    //       c[i][j] = 0;

    //   for(i=0; i<SIZE; i++) // Total Cycles: 7579000, Execs: 1000, Iters: 5
    //     for(j=0; j<SIZE; j++) // Total Cycles: 7520000, Execs: 5000, Iters: 5
    //       for(k=0; k<SIZE; k++) // Total Cycles: 7225000, Execs: 25000, Iters: 5
    //         c[i][j] += a[i][k] * b[k][j];
    //         cout << c[i][j] << "-";
    // }

    SW_port->Write(3,data);
    // SW_port->Request(1, 1, 3, 8);
    // if(SW_port->WaitForAcknowledge(1)){
    //   for(int kk=0;kk<27;kk++){
    //       SW_port->ReadData(data);
    //       cout << n << " - ";
    //     }
    //}

    return ;

  }
    

  SC_CTOR(Software) {
    result =0;
    for(unsigned i=0; i<SIZE; i++){
        for(unsigned j=0; j<SIZE; j++){
            a[i][j] =0;
            b[i][j] =0;
            c[i][j] =0;
        }
    }
    SC_METHOD(softwareFunction);
    
}

  
};
