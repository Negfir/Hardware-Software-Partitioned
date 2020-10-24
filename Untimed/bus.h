#include "systemc.h"
#include "interface.h"
#include <fstream>
#include <stdio.h>
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
        address =0;
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

    BusStatus status;
    bool ack;

    unsigned int buffer_data;

    unsigned int master_id;
    unsigned int address;
    unsigned int option;
    unsigned int length;
    unsigned int read_count;
    unsigned int write_count;
    unsigned timer;

    Bus_request next_req;   // request coming
    Bus_request cur_req;  // current scheduled request



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
        timer =0;
        buffer_data=0;
        ack=false;
        SC_THREAD(BusFunction);

    }
    SC_HAS_PROCESS(Bus);

    //################################ Master ################################//

    void Request(unsigned int mst_id, unsigned int addr, unsigned int op, unsigned int len)
    {     
      master_id=mst_id;
      address=addr;
      option=op;
      length=len;
      cout << "Got request from " << mst_id <<endl;
      return;
    }

    bool WaitForAcknowledge(unsigned int mst_id)
    {
    while(1){  
            cout << "waiting..."<<endl;
            wait(ack_minion);
            if(mst_id==master_id){  
                cout << "true"<<endl;
                    return true;
            }
        }
    }


    void WriteData(unsigned int data)
    {
        cout << "Write req"<<endl;
        buffer_data = data;
        data_valid.notify();
        wait(recieve);
        //cout << "data="<< data << " writen successfully! " << endl;
     return;
   
      }

    void ReadData(unsigned int &data)
    {

        data=buffer_data; 
        read_count++;
        cout << "data="<< data << " Read successfully! " << endl;
        data_valid.notify();
        wait(send);
        
        
        if(read_count==length){   // finish read
            read_count =0;
            //cout << "aaaaaaaaaaaaaaaaaaaaaa";
              
            
            return;
        }
      
 
    }



    //################################ Minion ################################//
    void Listen(unsigned int &req_addr, unsigned int &req_op, unsigned int &req_len)
    {     
        //cout << "Listening "<<req_op<<endl;
      req_addr=address;
      req_op=option;
      req_len=length;
      return;
    }

    void Acknowledge()
    {     
        cout << "Ack "<<endl;
      ack=true;
      ack_minion.notify();
      return;
    }

    void SendReadData(unsigned int data)
    {   
      
      buffer_data = data;
      cout << "sending " << buffer_data <<endl;
      send.notify();
      wait(data_valid);
      
      
      
      return;
    }

    void ReceiveWriteData(unsigned int &data)
    {     
        wait(data_valid);
        
        data = buffer_data;
        cout << "receiving " << data <<endl;
        write_count++;
        recieve.notify();
        if(write_count==length){
            write_count =0;
            
        }
      return;
    }

    void BusFunction()
    {


    }



  };


  #endif
