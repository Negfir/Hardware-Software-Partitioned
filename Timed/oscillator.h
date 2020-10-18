#include "systemc.h"


SC_MODULE(oscillator)
{
  sc_out<sc_logic> clk;

  void clockOscillator()
  {
    while(true)
    {
       clk.write(sc_logic_0);
       wait(5, SC_NS);
       clk.write(sc_logic_1);
       wait(5, SC_NS);
    }
  }

  SC_CTOR(oscillator)
  {
    SC_THREAD(clockOscillator);
  }

};