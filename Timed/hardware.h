#include "systemc.h"
#include "interface.h"
#include <queue>
#define CLOCK_CYCLE 20.0/3
//#define SIZE 6
#define LOOP 1000
#define a_ADDR 0
#define b_ADDR 36
#define c_ADDR 72
#define MEM_SIZE 108
//Operations
#define MEM_READ 0
#define MEM_Write 1
#define HW_OPs 2
#define CHECK_FLAG 3

//IDs
#define SW_id 0
#define HW_id 1


class InputAddress
{
public:

    unsigned int i;
    unsigned int j;

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

    unsigned int done_FLAG;

    unsigned int matrix_size;

    InputAddress input_address;
    InputAddress current_address;

    sc_port<bus_master_if> HW_master_port;
    sc_port<bus_minion_if> HW_minion_port;

    sc_in<sc_logic> clk;

    std::queue<InputAddress> queue;

    SC_CTOR(Hardware){
        
        c_reg=0;
        done_FLAG=0;
        SC_THREAD(hardwareMasterFunction);
        SC_THREAD(hardwareMinionFunction);

        sensitive << clk.pos();
    }



      //SC_HAS_PROCESS(hardware);

    void hardwareMasterFunction()
    {
      int mul_k_loop=0;
    while(1){
        

    if(!queue.empty()){
        current_address = queue.front();
        queue.pop();

        c_reg=0;
       cout <<endl<< "############## Starting Multiplication ################"<<endl;

        double loop5_start = sc_time_stamp().to_double ();
        HW_master_port->Request(HW_id, a_ADDR+current_address.i*SIZE, MEM_READ, SIZE);
        if(HW_master_port->WaitForAcknowledge(HW_id)){
          for(int k=0; k<SIZE;  k++){
            HW_master_port->ReadData(a_reg[k]);
            //cout << "###A is"<< a_reg[k] <<endl;
            }
          }

        for(int k=0; k<SIZE;  k++){
            HW_master_port->Request(HW_id, b_ADDR+k*SIZE+current_address.j, MEM_READ, 1);
            if(HW_master_port->WaitForAcknowledge(HW_id)){     
            HW_master_port->ReadData(b_reg[k]);
            //cout << "###B is"<< b_reg[k] <<endl;
            }
          }

          for(int k=0; k<SIZE;  k++){
            c_reg=c_reg+a_reg[k]*b_reg[k];
            cout  <<"Multiplying: "<<a_reg[k]<<" - "<<b_reg[k]<<endl;
          }
          cout <<endl << "c["<<current_address.i<<"]["<<current_address.j<<"] is:" << c_reg <<endl<<endl;
          
          HW_master_port->Request(HW_id, c_ADDR+(current_address.i)*SIZE+current_address.j, MEM_Write, 1);
          if(HW_master_port->WaitForAcknowledge(HW_id)){
            HW_master_port->WriteData(c_reg);
          }

        double loop5_end = sc_time_stamp().to_double ();
        mul_k_loop = mul_k_loop+ int((loop5_end-loop5_start)/CLOCK_CYCLE)+1;

          
          if (current_address.i==matrix_size-1 && current_address.j==matrix_size-1){
            done_FLAG=1;
            cout<<"Multiplying two K*1 marices cycles: "<<mul_k_loop/36<<endl;
            mul_k_loop=0;
            
            //cout <<"*******************************END**********************************"<<endl;
          }          
          //cout <<endl;

          

          

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
            matrix_size=in_address;
            //HW_minion_port->ReceiveWriteData(c_start);
            cout << "===================================================Request for HW from SW "<< input_address.i << input_address.j <<endl;
            start.notify();
            //wait(calcDone);
            

        }

        else if (in_option==CHECK_FLAG){
            HW_minion_port->Acknowledge();
            HW_minion_port->SendReadData(done_FLAG);
            
            //cout << "===================================================FLAG is: "<< done_FLAG <<endl;
            done_FLAG=0;
            //wait(calcDone);
            

        }


       }

    }




  };


