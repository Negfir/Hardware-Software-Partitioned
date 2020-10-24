#include "systemc.h"
#include "interface.h"
#define MEM_SIZE 108
#define a_ADDS 0
#define b_ADDR 36
#define c_ADRS 72
#define MEM_SIZE 108
#define MEM_READ 0
#define MEM_Write 1
#define OP 2



SC_MODULE (Hardware)
{
public:
    unsigned int in_address;
    unsigned int in_option;
    unsigned int in_length;
    unsigned int i,j;

    sc_port<bus_master_if> HW_master_port;
    sc_port<bus_minion_if> HW_minion_port;

    SC_CTOR(Hardware){
        

        SC_THREAD(hardwareMasterFunction);
        SC_THREAD(hardwareMinionFunction);
    }



      //SC_HAS_PROCESS(Memory);

    void hardwareMasterFunction()
    {
    // while(1){

    //     MEM_port->Listen(in_address, in_option, in_length);

    //     if (in_option==MEM_READ && (in_address+in_length-1)<MEM_SIZE){
    //         MEM_port->Acknowledge();
    //         //cout << "Addr is " << in_address <<endl; 
    //             for(int i=0;i<in_length;i++){  
    //                 MEM_port->SendReadData(MEM[in_address+i]);
    //                 cout << "address is" << in_address+i <<endl;
    //             }
    //     }
    //     else if (in_option==MEM_Write && (in_address+in_length-1)<MEM_SIZE){
    //         MEM_port->Acknowledge();
            
    //             for(int i=0;i<in_length;i++){  
    //                 MEM_port->ReceiveWriteData(MEM[in_address+i]);
    //                 cout << "Addr is" << in_address+i << " - " <<MEM[in_address+i]<<endl; 
    //             }


    //     }

    //   }

    }


    void hardwareMinionFunction()
    {
     while(1){

        HW_minion_port->Listen(in_address, in_option, in_length);

        if (in_option==OP){
            HW_minion_port->Acknowledge(); 
            HW_minion_port->ReceiveWriteData(i);
            HW_minion_port->ReceiveWriteData(j);
            //HW_minion_port->ReceiveWriteData(c_start);
            cout << "HW "<< i << j <<endl;
        }

        // else if (in_option==MEM_Write && (in_address+in_length-1)<MEM_SIZE){
        //     MEM_port->Acknowledge();
            
        //         for(int i=0;i<in_length;i++){  
        //             MEM_port->ReceiveWriteData(MEM[in_address+i]);
        //             cout << "Addr is" << in_address+i << " - " <<MEM[in_address+i]<<endl; 
        //         }


        // }

       }

    }




  };
