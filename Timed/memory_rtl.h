#include "systemc.h"
#include "interface.h"
#include <fstream>
#define MEM_SIZE 70000



class MEMORY_RTL: public sc_module
{

    

  public:

    unsigned int memData[MEM_SIZE];

    sc_in<sc_logic> clk;
    sc_in<sc_logic> Ren, Wen;
    sc_in<int> Addr;
    sc_in<int> DataIn;
    sc_out<int> DataOut;
    sc_out<sc_logic> Ack;

    SC_HAS_PROCESS(MEMORY_RTL);


    MEMORY_RTL(sc_module_name nm, char* file) : sc_module(nm)
    {
      ifstream init_file(file);
      int cnt= 0;
      int x;
       while (init_file >> x){
        memData[cnt++] = x;}

      SC_METHOD(rtl);
      sensitive << clk.pos();
    }



    void rtl()
    {
      
      if (Addr.read() < MEM_SIZE)
      {


        if (Wen.read() == sc_logic_1)
        {
          memData[Addr.read()] = DataIn.read();
          cout << "data="<< DataIn.read() << " writen successfully! " << endl;

        }

        else if(Ren.read() == sc_logic_1)
        {
         
          DataOut.write(memData[Addr.read()]);
          //cout << "data="<< DataIn.read() << " read successfully! " << endl;
        }


        Ack.write(sc_logic_1);
      }

      else
      {
        Ack.write(sc_logic_0);
        cout << "!!! Memory out of bound !!!" <<endl;
      }


      // make ack signal zero after read/wrtite is done
      if (Wen.read() == sc_logic_0 && Ren.read() == sc_logic_0)
      {
        Ack.write(sc_logic_0);
        return;
      }



    }





};