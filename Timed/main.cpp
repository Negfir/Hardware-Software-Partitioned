
#include "systemc.h"
#include "interface.h"
#include "memory.h"
#include "bus.h"
#include "software.h"
#include "hardware.h"
#include <fstream>



class Top: public sc_module
{
    
public:
    
    //sc_event ACK2SAD;
    sc_signal<sc_logic> clk_sig;
    //sc_signal<sc_logic> Clk;
    sc_signal<sc_logic> Rst;
    oscillator osc;

    Software* sw_obj;
    Hardware* hw_obj;
    Bus* bus_obj;
    Memory* mem_obj;
    
    SC_HAS_PROCESS(Top);

    Top(sc_module_name nm): sc_module(nm), osc("oscillator"){
        
    std::string name ="";
      Software* sw_obj;
      //Hardware* hw_obj;
      Bus* bus_obj;
     Memory* mem_obj;

     osc.clk(clk_sig);

      name = "Hardware";
      hw_obj = new Hardware(name.c_str());
          
      name = "Bus";
      bus_obj = new Bus(name.c_str());
      
      name= "Software";
      sw_obj = new Software(name.c_str());
      
      name = "Memory";
      mem_obj = new Memory(name.c_str());

      sw_obj->SW_port.bind(*bus_obj);
      hw_obj->HW_master_port.bind(*bus_obj);
      hw_obj->HW_minion_port.bind(*bus_obj);
      mem_obj->MEM_port.bind(*bus_obj);
      
      bus_obj->clk(clk_sig);
      hw_obj->clk(clk_sig);
      sw_obj->clk(clk_sig);
      mem_obj->clk(clk_sig);

        
    }
    
};



int sc_main(int argc, char* argv[]) {
  // Software SW("SW");
  // Memory mem("mem");
  // Bus bus("bus");
  // SW.SW_port(bus);
  // mem.MEM_port(bus);
std::string name= "TopModule";
    Top *top_obj = new Top(name.c_str());
        
    sc_start();

   //sc_start();

  return(0);
}

