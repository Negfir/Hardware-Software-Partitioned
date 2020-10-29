#include "systemc.h"
#include "interface.h"
#include <fstream>
#include <stdio.h>
#include "oscillator.h"
#include <queue>
#define MEM_SIZE 108

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

    sc_signal<sc_logic> clk_sig;
    sc_in<sc_logic> clk;
    // memory(sc_module_name nm) : sc_module(nm)
    // {
    //   ifstream init_file(file);
    //   int cnt= 0;
    //   int x;
    //    while (init_file >> x){
    //     memData[cnt++] = x;


    //   }
    // }
    Bus(sc_module_name nm):sc_module(nm)
    {
        //bus_status = UNLOCK;  
       // osc.clk(clk_sig);
        timer =0;
        buffer_data=0;
        ack=false;
        status=UNLOCK;
        SC_THREAD(BusFunction);
        
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
            cout <<cur_req.address<< "-"<<cur_req.option<< "-"<<cur_req.length<<endl;
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

    void BusFunction()
    {
        //while(true){
        //wait(request);
        //busToRequest.notify();
        ///cout << "notify " ;
    //    cout << "add " << cur_req.address;     
    // unsigned next_schedu_ID =1;
    // if(!queue.empty()){
    //     cout << "not empty " ;
    //     cur_req = queue.front();
    // }
    // bool find_flag =false;

    

        while(true){
            //wait(request);
            if(!queue.empty()){
                cout << "size is " <<queue.size()<<endl;
                cur_req = queue.front();
                //cout << "Cur is " << cur_req.length<<endl;
                queue.pop();
                //cout << "in Q";
                busToListen.notify();

                wait(busUnlock);
            }
            else{
                //cout << "no req";
                wait(request);
                //cout << "no req";
            }
        }


    //}

}

  };


  #endif
