#include "systemc.h"
#include "interface.h"
#include <fstream>
#define MEM_SIZE 70000


class memory: public sc_module, public bus_master_if, public bus_minion_if
{

    unsigned int memData[MEM_SIZE]={0};


    memory(sc_module_name nm, char* file) : sc_module(nm)
    {
      ifstream init_file(file);
      int cnt= 0;
      int x;
       while (init_file >> x){
        memData[cnt++] = x;


      }


    }
    bool Write(unsigned int addr, unsigned int data)
    {
      if (addr < MEM_SIZE)
      {
        memData[addr]=data;
        cout << "data="<< data << " writen successfully! " << endl;
        return true;
      }
      cout << "!!! Writing failed !!!" <<endl;
      return false;
      }

    bool Read(unsigned int addr, unsigned int& data)
    {
      if (addr < MEM_SIZE)
      {
        data=memData[addr];
        
        return true;
      }
      cout << "!!! Reading failed !!!" <<endl;
      return false;
      }


  };
