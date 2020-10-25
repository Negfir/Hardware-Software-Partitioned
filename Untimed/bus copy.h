#include "systemc.h"
#include "interface.h"
#include <fstream>
#include <deque>
#include <stdio.h>
#define MEM_SIZE 108

#ifndef __bus_H_INCLUDED__   
#define __bus_H_INCLUDED__
#define CYCLE_TIME 100.0/15

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

    sc_event request;
    sc_event DataReady;
    sc_event DataDone;
    sc_event ack2bus;
    sc_event ack2master;
    sc_event bus_unlock;
    sc_event req2bus;

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
    std::deque<Bus_request> queue;

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
        //status = UNLOCK;  
        timer =0;
        buffer_data=0;
        ack=false;
        SC_THREAD(BusFunction);

    }
    SC_HAS_PROCESS(Bus);

    //################################ Master ################################//

    void Request(unsigned int mst_id, unsigned int addr, unsigned int op, unsigned int len)
    {     
      cur_req.master_id=mst_id;
      cur_req.address=addr;
      cur_req.option=op;
      cur_req.length=len;
      cout << "Got request from " << mst_id <<endl;
      queue.push_back(next_req);
      //cur_req=
      request.notify();
      //return;
    }

    bool WaitForAcknowledge(unsigned int mst_id)
    {
    while(1){  
            cout << "waiting..."<<cur_req.master_id<<endl;
            wait(ack_minion);
            if(mst_id==cur_req.master_id){  
                cout << "true"<<endl;
                    return true;
            }
        }

    // while(1){  // keep watching until bus ask me
    //     wait(ack2master);
    //     //if(cur_req!=NULL){
    //         if(mst_id==cur_req.master_id){   // if scheduled is me
    //             if(status==ERROR){   // timeout
    //                 //status = UNLOCK;
    //                 bus_unlock.notify();
    //                 return false;
    //             }
    //             else //if(status==UNLOCK){  // acknowledged by slave
    //                 return true;
    //             //}
    //         }
    
    // }


    }


    void WriteData(unsigned int data)
    {
        cout << "Write req"<<endl;
        buffer_data = data;
        data_valid.notify();
        wait(recieve);


        //wait(1*CYCLE_TIME,SC_NS);
        
        // buffer_data = data;
        // DataReady.notify();
        
        // wait(0,SC_NS);


     return;
   
      }

    void ReadData(unsigned int &data)
    {

        data=buffer_data; 
        cur_req.read_count++;
        cout << "data="<< data << " Read successfully! " << endl;
        data_valid.notify();
        wait(send);
        
        
        if(cur_req.read_count==cur_req.length){   // finish read
            cur_req.read_count =0;
            //cout << "aaaaaaaaaaaaaaaaaaaaaa";
              
            
            return;
        }


        //wait(2*CYCLE_TIME,SC_NS);
        
        // data = buffer_data;
        // cur_req.read_count ++;
        
        // if(cur_req.read_count==cur_req.length){   // finish read
        //     cur_req.read_count =0;
        //     status=UNLOCK;
            
        //     DataDone.notify();   // this must be notify earier than bus_unlock, otherwise memroy cannot catch request
            
        //     wait(0,SC_NS);
        //     bus_unlock.notify();
            
        //     return ;
        // }
        
        // DataDone.notify();
        // wait(0,SC_NS);
        
        // return ;
 
    }



    //################################ Minion ################################//
    void Listen(unsigned int &req_addr, unsigned int &req_op, unsigned int &req_len)
    {     
      //cout << "Listening "<<req_op<<endl;
      //wait(request);
      req_addr=cur_req.address;
      req_op=cur_req.option;
      req_len=cur_req.length;
      //return;
    }

    void Acknowledge()
    {     
        cout << "Ack "<<endl;
      ack=true;
      ack_minion.notify();

    
    // status = LOCK;    // have issue when lock here
    // timer = 0; // clean timer
    // ack2bus.notify();

      //return;
    }

    void SendReadData(unsigned int data)
    {   
      
      buffer_data = data;
      cout << "sending " << buffer_data <<endl;
      send.notify();
      wait(data_valid);
   //  buffer_data = data;
    
   // // if(read_send_count==cur_req.length){
   // //     read_send_count =0;
   // //     return true;
   // // }
   // // else{
   //      wait(DataDone);
      
      
      return;
    }

    void ReceiveWriteData(unsigned int &data)
    {     
        wait(data_valid);
        
        data = buffer_data;
        cout << "receiving " << data <<endl;
        cur_req.write_count++;
        recieve.notify();
        if(cur_req.write_count==cur_req.length){
            cur_req.write_count =0;
            
        }

      //   wait(DataReady);
      //   data = buffer_data;
        
      //   cur_req.write_count ++;
        
      //   if(cur_req.write_count==cur_req.length){
      //       cur_req.write_count =0;
      //       status=UNLOCK;
      //       bus_unlock.notify();
      //   }

      // return;
    }

    void BusFunction()
    {
    // unsigned next_schedu_ID =1;
    // if(!queue.empty()){
    //     cur_req = queue.at(0);

    // }
    // bool find_flag =false;
    
    // while(1){
    //     //if(status==UNLOCK){ // only schedulr new request when bus unlock
    //         if(!queue.empty()){
                
    //             while(1){  // until find a request to schedule
    //                 for(unsigned i=0;i<queue.size();i++){
    //                     if (queue.at(i).master_id==next_schedu_ID){  // find the target
    //                         cur_req = queue.at(i);
    //                         queue.erase(queue.begin()+i);
    //                         find_flag =true;
    //                         break;
    //                     }
    //                 }
    //                 next_schedu_ID =next_schedu_ID%2+1;  // for Round Robin
    //                 if(find_flag==true){
    //                     find_flag = false;
    //                     break;
    //                 }
    //             }
            
    //             //cout<<"current schedule is: master "<<cur_req.master_id<<endl;
    //             status=WAIT;
                
    //             wait(0,SC_NS); // let slave start listening
    //             request.notify();   // the current scheduled request is ready, search for slave
                
    //             sc_time AnswerTime = sc_time(0,SC_NS);
    //             wait(AnswerTime,ack2bus);
    //             if(status==LOCK){    // the slave answer
    //                 ack2master.notify();
    //                 //cout<<"ack to "<<cur_req.master_id<<endl;
    //             }
    //             else{
    //                 ack2master.notify();
    //                 status=ERROR;
    //                 cout<<"master "<<cur_req.master_id<<"cannot find "<<cur_req.address<<" address!"<<endl;
    //             }
                
    //             wait(bus_unlock);  // if there is transmission
    //         }
    //         else{
    //             //wait(0,SC_NS); // cannot use this, because it will make sw get stuck when adding wait in sw
    //             wait(req2bus);    // if no transmission
    //         }
    //     //}
        
    // }

    }



  };


  #endif
