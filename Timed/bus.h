#include "systemc.h"
#include "interface.h"
#include <fstream>
#include <stdio.h>
#include "oscillator.h"
#include <queue>
#define MEM_SIZE 108
#define CLOCK_CYCLE 20.0/3

#ifndef __bus_H_INCLUDED__   
#define __bus_H_INCLUDED__

enum BusStatus {
    UNLOCK,
    WAIT,
    LOCK,
    ERROR
};

class Bus_request
{
public:

    unsigned int master_id;
    unsigned int address;
    unsigned int option;
    unsigned int length;
    unsigned int read_count;
    unsigned int write_count;

    
    // default constructor
    Bus_request()
    {
        read_count =0;
        write_count =0;

        master_id =0;
        address =999;
        option =0;
        length =0;
    }
    
};


class Bus: public sc_module, public bus_master_if, public bus_minion_if
{

public:
    

    sc_event ack_minion;
    sc_event data_valid;
    sc_event send;
    sc_event recieve;
    sc_event request;
    sc_event busToRequest;
    sc_event busToListen;
    sc_event busUnlock;

    BusStatus status;
    bool ack;

    unsigned int buffer_data;

    unsigned int master_id;
    unsigned int address;
    unsigned int option;
    unsigned int length;
    unsigned int read_count;
    unsigned timer;

    std::queue<Bus_request> queue;

    Bus_request next_req;   // request coming
    Bus_request cur_req;  // current scheduled request

;
    sc_in<sc_logic> clk;

    Bus(sc_module_name nm):sc_module(nm)
    {
        //bus_status = UNLOCK;  
       // osc.clk(clk_sig);
        timer =0;
        buffer_data=0;
        ack=false;
        status=UNLOCK;
        SC_THREAD(Arbiter);
        
        sensitive << clk.pos();  


    }
    SC_HAS_PROCESS(Bus);

    //################################ Master ################################//

    void Request(unsigned int mst_id, unsigned int addr, unsigned int op, unsigned int len)
    { 
    //wait(busToRequest);

      next_req.master_id=mst_id;
      next_req.address=addr;
      next_req.option=op;
      next_req.length=len;

      
      queue.push(next_req);
      //cout << "Got request from " << queue.front().address <<endl;
      
      request.notify();
      //return;
    }

    bool WaitForAcknowledge(unsigned int mst_id)
    {
    while(1){  
            //cout << "WaitForAcknowledge"<<endl;
            
            wait(ack_minion);
            //cout <<cur_req.address<< "-"<<cur_req.option<< "-"<<cur_req.length<<endl;
            if(mst_id==cur_req.master_id){  
                    ack=false;
                    return true;
            }
        }
    }


    void WriteData(unsigned int data)
    {
        //cout << "WriteData"<<endl;
        buffer_data = data;
        data_valid.notify();
        wait(recieve);
        //cout << "data="<< data << " writen successfully! " << endl;
     return;
   
      }

    void ReadData(unsigned int &data)
    {

        data=buffer_data; 
        cur_req.read_count++;
        //cur_req.address++;
        //cout << "data="<< data << " Read successfully! " <<cur_req.read_count<< cur_req.length<<endl;
        wait(2*CLOCK_CYCLE, SC_NS); //Read wait
        data_valid.notify();
        
        
        if(cur_req.read_count==cur_req.length){   // finish read
            cur_req.read_count =0;
            //cout << "aaaaaaaaaaaaaaaaaaaaaa";
              busUnlock.notify();
            
            return;
        }
      
        wait(send);
        
    }



    //################################ Minion ################################//
    void Listen(unsigned int &req_addr, unsigned int &req_op, unsigned int &req_len)
    {     
        //cout << "listening.."<<cur_req.address<<endl;
      wait(busToListen)  ;
      req_addr=cur_req.address;
      req_op=cur_req.option;
      req_len=cur_req.length;
      //return;
    }

    void Acknowledge()
    {     
        //cout << "Acknowledge"<<endl;
      wait(CLOCK_CYCLE, SC_NS); //Acknowledge wait
      ack=true;
      ack_minion.notify();
      return;
    }

    void SendReadData(unsigned int data)
    {   
      
      buffer_data = data;
      //cout << "sending " << buffer_data <<endl;
      send.notify();
      wait(data_valid);
      
      
      
      return;
    }

    void ReceiveWriteData(unsigned int &data)
    {     
        wait(data_valid);
        wait(CLOCK_CYCLE, SC_NS); // Write wait
        data = buffer_data;
        //cout << "receiving " << data <<endl;
         cur_req.read_count++;
         //cur_req.address++;
        recieve.notify();
        if(cur_req.read_count==cur_req.length){
            cur_req.read_count =0;
            busUnlock.notify();
        }
      return;
    }

    void Arbiter()
    {


    
        while(true){
            //wait(request);
            if(!queue.empty()){
                //cout << "Buffer size is " <<queue.size()<<endl;
                wait(2*CLOCK_CYCLE, SC_NS); //Request wait

                cur_req = queue.front();
            
                queue.pop();
            
                busToListen.notify();

                wait(busUnlock);
            }
            else{
              
                wait(request);
          
            }
        }


    //}

}

  };


  #endif
