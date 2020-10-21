#include "systemc.h"
#include "interface.h"
#include <fstream>
#define MEM_SIZE 70000


class Bus: public sc_module, public bus_master_if, public bus_minion_if
{

public:
    
    sc_event request;
    sc_event DataReady;
    sc_event DataDone;
    sc_event ack2bus;
    sc_event ack2master;
    sc_event bus_unlock;
    sc_event req2bus;

    unsigned req_id;
    unsigned address;
    unsigned option;
    unsigned length;
    
    Bus_request mst_request;   // request coming
    Bus_request curr_schedule;  // current scheduled request
    Bus_lock_status bus_status;  // lock or unlock
    unsigned timer;


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
        
        //SC_THREAD(Bus_process);

    }
    SC_HAS_PROCESS(Bus);

    void Bus::Request(unsigned mst_id, unsigned addr, unsigned op, unsigned len)
    {
        //wait(2*CYCLE_TIME,SC_NS);  // put it in begginning, otherwise it will be scheduled before notify
        
        req_id = mst_id;
        address = addr;
        option = op;
        length =len;

        // queue.push_back(mst_request);
        
        //cout<<"push request of master "<<mst_id<<" into Q"<<endl;
        
        req2bus.notify();
    }

    bool WaitForAcknowledge(unsigned mst_id)
    {
        while(1){  // keep watching until bus ask me
            wait(ack2master);
            //if(curr_schedule!=NULL){
                if(mst_id==req_id){   // if scheduled is me

                        return true;
                    //}
                }
       
        }
    }

    bool ReadData(unsigned &data)
    {
        
        data = buffer_data;
        
            
        DataDone.notify();   // this must be notify earier than bus_unlock, otherwise memroy cannot catch request

        bus_unlock.notify();
            
        
        DataDone.notify();
  
        
        return true;
    }

    bool WriteData(unsigned data)
    {
        
        buffer_data = data;
        DataReady.notify();
        
        return true;
    }



    //################################ Minion ################################//

    void Listen(unsigned &req_addr, unsigned &req_op, unsigned &req_len)
    {
        wait(request);
        req_addr = address;
        req_op = option;
        req_len = length;
        
    }
    

    void Acknowledge()
    {
        
        //bus_status = LOCK;    // have issue when lock here
        timer = 0; // clean timer
        ack2master.notify();

    }

    bool SendReadData(unsigned data)
    {
       // read_send_count++;  
        buffer_data = data;
          
        
            return true;
       // }
    }

    bool ReceiveWriteData(unsigned &data)
    {
        wait(DataReady);
        data = buffer_data;
        
        return true;
    }



  };
