#include "systemc.h"
#include "interface.h"
#include "bus.h"
#include <fstream>
#include <stdio.h>
#define MEM_SIZE 108
#define a_ADDS 0
#define b_ADDR 36
#define c_ADRS 72
#define MEM_SIZE 108
#define MEM_READ 0
#define MEM_Write 1
#define HW_MUL 2




SC_MODULE (Memory)
{
public:
    unsigned in_address;
    unsigned in_option;
    unsigned in_length;

    unsigned int MEM[MEM_SIZE]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9,
             0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9,
             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    // unsigned a[MEM_SIZE];
    // unsigned b[MEM_SIZE];
    // unsigned c[MEM_SIZE];

    sc_port<bus_minion_if> MEM_port;

    SC_CTOR(Memory){
        // MEM[108]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9,
        //      0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9,
        //      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        // unsigned int a[MEM_SIZE]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9};
        // unsigned int b[MEM_SIZE]={0,0,0,0,0,0,0,0,9,4,7,9,0,12,14,15,16,11,0,2,3,4,5,6,0,4,3,2,1,2,0,2,7,6,4,9};
        

        SC_THREAD(memoryFunction);
    }



      //SC_HAS_PROCESS(Memory);

    void memoryFunction()
    {
    while(1){

        MEM_port->Listen(in_address, in_option, in_length);

        if (in_option==MEM_READ && (in_address+in_length-1)<MEM_SIZE){
            MEM_port->Acknowledge();
            //cout << "Addr is " << in_address <<endl; 
                for(int i=0;i<in_length;i++){  
                    MEM_port->SendReadData(MEM[in_address+i]);
                    cout << "address is" << in_address+i <<endl;
                }
        }
        else if (in_option==MEM_Write && (in_address+in_length-1)<MEM_SIZE){
            MEM_port->Acknowledge();
            
                for(int i=0;i<in_length;i++){  
                    MEM_port->ReceiveWriteData(MEM[in_address+i]);
                    cout << "Addr is" << in_address << " - " <<MEM[in_address+i]<<endl; 
                }


        }

      }

    }




  };
