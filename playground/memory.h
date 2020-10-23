#include "systemc.h"
#include "interface.h"
#include <fstream>
#define MEM_SIZE 70000

#ifndef __memory_H_INCLUDED__   
#define __memory_H_INCLUDED__


class memory: public sc_module, public bus_master_if, public bus_minion_if
{
  private:
    unsigned int memData[MEM_SIZE]={0};

  public:

    memory(sc_module_name nm, char* file) : sc_module(nm)
    {
      ifstream init_file(file);
      int cnt= 0;
      int x;
       while (init_file >> x){
        memData[cnt++] = x;
      }
    }

    //################################ Master ################################//

    void Request(unsigned int mst_id, unsigned int addr, unsigned int op, unsigned int len)
    {     
      cout << "hi";
    }

    bool WaitForAcknowledge(unsigned int mst_id)
    {
      cout<<"ack";

    }


    void WriteData(unsigned int data)
    {

      cout << "data="<< data << " writen successfully! " << endl;
     return;
   
      }

    void ReadData(unsigned int &data)
    {

      cout << "!!! Reading failed !!!" <<endl;
      return;
    }

    //################################ Minion ################################//
    void Listen(unsigned int &req_addr, unsigned int &req_op, unsigned int &req_len)
    {     
      cout << "listen";
    }

    void Acknowledge()
    {     
      cout << "ack";
    }

    void SendReadData(unsigned int data)
    {     
      cout << "send";
    }

    void ReceiveWriteData(unsigned int &data)
    {     
      cout << "recieve";
    }


  };


#endif