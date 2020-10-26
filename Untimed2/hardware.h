#include "systemc.h"
#include "interface.h"
#include <queue>

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


class InputAddress
{
public:

    unsigned int i;
    unsigned int j;

    
    // default constructor
    InputAddress()
    {
      i=0;
      j=0;
    }
    
};


SC_MODULE (Hardware)
{
public:
    unsigned int in_address;
    unsigned int in_option;
    unsigned int in_length;
    unsigned int i,j;
    sc_event start;
    sc_event calcDone;

    unsigned int a_reg[SIZE];
    unsigned int b_reg[SIZE];
    unsigned int c_reg;

    InputAddress input_address;
    InputAddress current_address;

    sc_port<bus_master_if> HW_master_port;
    sc_port<bus_minion_if> HW_minion_port;

    std::queue<InputAddress> queue;

    SC_CTOR(Hardware){
        
        c_reg=0;
        SC_THREAD(hardwareMasterFunction);
        SC_THREAD(hardwareMinionFunction);
    }



      //SC_HAS_PROCESS(hardware);

    void hardwareMasterFunction()
    {
    while(1){
        

    if(!queue.empty()){
        current_address = queue.front();
        queue.pop();

        c_reg=0;
        cout << "#######Starting Mul#########"<<endl;

        HW_master_port->Request(HW_id, a_ADDR+current_address.i*SIZE, MEM_READ, SIZE);
        if(HW_master_port->WaitForAcknowledge(HW_id)){
          for(int k=0; k<SIZE;  k++){
            HW_master_port->ReadData(a_reg[k]);
            cout << "###A is"<< a_reg[k] <<endl;
            }
          }

        for(int k=0; k<SIZE;  k++){
            HW_master_port->Request(HW_id, b_ADDR+k*SIZE+current_address.j, MEM_READ, 1);
            if(HW_master_port->WaitForAcknowledge(HW_id)){     
            HW_master_port->ReadData(b_reg[k]);
            cout << "###B is"<< b_reg[k] <<endl;
            }
          }

          for(int k=0; k<SIZE;  k++){
            c_reg=c_reg+a_reg[k]*b_reg[k];
            cout << "******************"<<a_reg[k]<<" - "<<b_reg[k]<<endl;
          }
          cout << "******************c["<<current_address.i<<"]["<<current_address.j<<"] is:" << c_reg << endl;
          calcDone.notify();

      }
      else{
        wait(start);
      }
      }
    }


    void hardwareMinionFunction()
    {
     while(1){

        HW_minion_port->Listen(in_address, in_option, in_length);

        if (in_option==HW_OPs){
            HW_minion_port->Acknowledge(); 
            HW_minion_port->ReceiveWriteData(input_address.i);
            HW_minion_port->ReceiveWriteData(input_address.j);
            queue.push(input_address);
            //HW_minion_port->ReceiveWriteData(c_start);
            cout << "===================================================HW "<< i << j <<endl;
            start.notify();
        }
        //wait(calcDone);
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
