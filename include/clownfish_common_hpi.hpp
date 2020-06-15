#pragma once
#include "systemc.h"
#include <iostream>

/*
   basic fliflop model
*/
template<int size>
SC_MODULE(FliFlop)
{
   sc_in  <sc_bit> clk ,reset;
   sc_in  <sc_bv <size> > d;
   sc_out <sc_bv <size> > q;

   void proc_ff();

   SC_CTOR(FlipFlopBit)
   {
      SC_METHOD(proc_ff);
      sensitive_pos << clk << reset;
   }
};


