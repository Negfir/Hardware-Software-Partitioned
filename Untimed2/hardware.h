#include "systemc.h"
#include "interface.h"
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


SC_MODULE (Hardware)
{
public:
    unsigned int in_address;
    unsigned int in_option;
    unsigned int in_length;
    unsigned int i,j;
    sc_event start;

    unsigned int a_reg[SIZE];
    unsigned int b_reg[SIZE];
    unsigned int c_reg;

    sc_port<bus_master_if> HW_master_port;
    sc_port<bus_minion_if> HW_minion_port;

    SC_CTOR(Hardware){
        

        SC_THREAD(hardwareMasterFunction);
        SC_THREAD(hardwareMinionFunction);
    }



      //SC_HAS_PROCESS(Memory);

    void hardwareMasterFunction()
    {
    while(1){
        wait(start);
        cout << "#######Starting Mul#########"<<endl;

        HW_master_port->Request(HW_id, a_ADDS+i*SIZE, MEM_READ, SIZE);
        if(HW_master_port->WaitForAcknowledge(HW_id)){
          for(int k=0; k<SIZE;  k++){
            HW_master_port->ReadData(a_reg[i*SIZE+k]);
            cout << "#A is"<< a_reg[i*SIZE+k] <<endl;
            }
          }


      }

    }


    void hardwareMinionFunction()
    {
     while(1){

        HW_minion_port->Listen(in_address, in_option, in_length);

        if (in_option==HW_OPs){
            HW_minion_port->Acknowledge(); 
            HW_minion_port->ReceiveWriteData(i);
            HW_minion_port->ReceiveWriteData(j);
            //HW_minion_port->ReceiveWriteData(c_start);
            cout << "HW "<< i << j <<endl;
            start.notify();
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
