#include "systemc.h"
#include "interface.h"
#include "bus.h"
#include <fstream>
#define SIZE 6
#define LOOP 3
#define a_ADDR 0
#define b_ADDR 36
#define c_ADDR 72
#define MEM_SIZE 108
//Operations
#define MEM_READ 0
#define MEM_Write 1
#define HW_OPs 2
//IDs
#define SW_id 0
#define HW_id 1


SC_MODULE (Software)
//class Software: public sc_module
{
    sc_port<bus_master_if> SW_port;

    unsigned a[SIZE][SIZE];
    unsigned b[SIZE][SIZE];
    unsigned c[SIZE][SIZE];
    
    
    unsigned result;
    


  void softwareFunction() 
  {
    int n;
    int i,j,k;

    unsigned int data=0;

    for (n=0; n<LOOP; n++) // Total Cycles: 8186006, Execs: 1, Iters: 1000
    {

    SW_port->Request(SW_id, c_ADRS, MEM_Write, 36);
    if(SW_port->WaitForAcknowledge(SW_id)){
      for(i=0; i<SIZE; i++) // Total Cycles: 579000, Execs: 1000, Iters: 5
        {
        for(j=0; j<SIZE; j++) // Total Cycles: 520000, Execs: 5000, Iters: 5
            {
            //c[i][j] = 0;
            cout <<"aaaaa";
            SW_port->WriteData(0);
            }
        }
    }
      for(i=0; i<SIZE; i++) // Total Cycles: 7579000, Execs: 1000, Iters: 5
        for(j=0; j<SIZE; j++) // Total Cycles: 7520000, Execs: 5000, Iters: 5
        {
            SW_port->Request(SW_id,0, HW_OPs, 2);
            if(SW_port->WaitForAcknowledge(SW_id)){
                SW_port->WriteData(i);
                SW_port->WriteData(j);
                //cout <<"====="<<data;
            }


          for(k=0; k<SIZE; k++) // Total Cycles: 7225000, Execs: 25000, Iters: 5
          {
            c[i][j] += a[i][k] * b[k][j];
            //cout  <<n<<"-"<< i<<"-"<<j<<"-"<<k<<"-"<<endl;
        }
    }
    }

// SW_port->Request(SW_id, c_ADRS, 0, 1);
// if(SW_port->WaitForAcknowledge(SW_id)){
//     SW_port->WriteData(4);
//     }

// SW_port->Request(SW_id, c_ADRS+5, MEM_Write, 1);
// if(SW_port->WaitForAcknowledge(SW_id)){
//     SW_port->WriteData(6);
//     }

    //SW_port->WriteData(3);
    // SW_port->Request(0, 28, 0, 4);
    // //SW_port->ReadData(data);
    // if(SW_port->WaitForAcknowledge(0)){
    //     // SW_port->WriteData(0);
    //     // SW_port->WriteData(2);
    //     // SW_port->WriteData(3);
    //     // SW_port->WriteData(4);
    //   for(int kk=0;kk<4;kk++){
    //       SW_port->ReadData(data);
    //       cout <<"==="<< data << endl;
    //     }


    // }

    // SW_port->Request(0, 6, 1, 2);
    // //SW_port->ReadData(data);
    // if(SW_port->WaitForAcknowledge(0)){
    //     SW_port->WriteData(7);
    //     SW_port->WriteData(8);
    // }

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
    SC_THREAD(softwareFunction);
    
}

  
};