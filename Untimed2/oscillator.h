#include "systemc.h"
#define CLOCK_CYCLE 100.0/15

#ifndef __oscillator_H_INCLUDED__   
#define __oscillator_H_INCLUDED__

SC_MODULE(oscillator)
{
  sc_out<sc_logic> clk;

  void clockOscillator()
  {
    while(true)
    {
       clk.write(sc_logic_0);
       wait(CLOCK_CYCLE/2, SC_NS);
       clk.write(sc_logic_1);
       wait(CLOCK_CYCLE/2, SC_NS);
    }
  }

  SC_CTOR(oscillator)
  {
    SC_THREAD(clockOscillator);
  }

};

#endif