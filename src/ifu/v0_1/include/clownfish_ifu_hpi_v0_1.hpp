#pragma once
#include "systemc.h"
#include <iostream>

template<int size>
SC_MODULE(FliFlop)
{
   sc_in  <sc_bit<size> > d, clk;
   sc_out <sc_bit<size> > q;

   void proc_ff();

   SC_CTOR(FlipFlopBit)
   {
      SC_METHOD(proc_ff);
      sensitive_pos << clk;
   }
};


