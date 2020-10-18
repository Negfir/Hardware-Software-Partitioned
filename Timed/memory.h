#include "systemc.h"
#include "interface.h"
#include "memory_rtl.h"
#include "oscillator.h"
#include <fstream>
#define MEM_SIZE 70000

class memory: public sc_module, public simple_mem_if
{

  
  public:
    sc_signal<sc_logic> clk_sig;
    sc_signal<sc_logic>ren_sig, wen_sig, ack_sig;
    sc_signal<int> addr_sig;
    sc_signal<int> dataIn_sig;
    sc_signal<int> dataOut_sig;

  
    MEMORY_RTL memory_rtl;
    oscillator osc;

    memory(sc_module_name nm, char* file) : sc_module(nm), osc("oscillator"), memory_rtl("MEMORY_RTL",(char *)file)
    {

      osc.clk(clk_sig);

      ren_sig.write(sc_logic_0);
      wen_sig.write(sc_logic_0);
      ack_sig.write(sc_logic_0);

      memory_rtl.clk(clk_sig);
      memory_rtl.Ren(ren_sig);
      memory_rtl.Wen(wen_sig);
      memory_rtl.Addr(addr_sig);
      memory_rtl.DataIn(dataIn_sig);
      memory_rtl.DataOut(dataOut_sig);
      memory_rtl.Ack(ack_sig);


    }

    // void oscillator() {
    //   while(true)
    //   {
    //      clk_sig.write(sc_logic_0);
    //      wait(5, SC_NS);
    //      clk_sig.write(sc_logic_1);
    //      wait(5, SC_NS);
    //   }
    // }


    bool Write(unsigned int addr, unsigned int data)
    {     
      
      addr_sig.write(addr);
      dataIn_sig.write(data);

      wait(10, SC_NS); //wait for address to be valid

      wen_sig.write(sc_logic_1);
      

      wait(10, SC_NS); // wait for the data to be written on memory

      bool ack = ack_sig.read() == sc_logic_1;
      wen_sig.write(sc_logic_0);

      
      //cout << "Writing " << dataIn_sig<< " on address:" << addr_sig << " ack is:" << ack <<endl;
      return ack;

      }

    bool Read(unsigned int addr, unsigned int& data)
    {

      
      ren_sig.write(sc_logic_1); 
      addr_sig.write(addr);
      
      wait(10, SC_NS); //wait for address to be valid

      data = dataOut_sig.read();

      wait(10, SC_NS); // wait for read data to be available on bus

      bool ack = ack_sig.read() == sc_logic_1;
      ren_sig.write(sc_logic_0);

      

      return ack;


      }

  };
