#pragma once
#include "systemc.h"
#include "clownfish_ifu_hpi.hpp"
#include "clownfish_ifu_parameters.hpp"
#include <stdint.h>


enum <int>{
  IDLE  = 0,
  VALID,
  HOLD 
} AddrMngSate;

template<int xlen>
SC_MODULE(Address_Manager)
{
    // input 
   sc_in <sc_bit> clk, reset;
   sc_in <sc_bv <xlen> > pc;
   sc_in <sc_bit>  valid_from_pcgen;
   sc_in <sc_bit> iccm_ready, ibu_ready  
   sc_in <sc_bit> eu_flush_enable;
   sc_in <sc_bv <xlen> > eu_flush_pc;

   // output
   sc_out <sc_bit> iccm_valid; ibu_valid;
   sc_out <sc_bv <xlen> > address;
   sc_out <sc_bit> ready_to_pcgen;

   //inter signals 
   //FSM
   AddrMngSate state;

   //FF
   sc_signal <sc_bv <xlen> > pc_q;
   sc_signal <sc_bit> valid_q;
   sc_signal <sc_bit> flush_pc_q;
   sc_signal <sc_bit> flush_enable_q;

   void AddrMngStateTransfer();
   void AddrMngOutput();

   FliFlop<xlen> *ff_pc;
   FliFlop<1> *ff_valid;
   FliFlop<xlen> *ff_flush_pc;
   FliFlop<1> *ff_flush_enable;

   SC_CTOR(Address_Manager)
   {
       SC_METHOD(AddrMngStateTransfer);
       sensitive_pos <<clk<<reset;
       sensitive_pos <<valid_q;
       sensitive_pos <<flush_enable_q;
       sensitive_pos <<iccm_ready<<ibu_ready;
       sensitive_pos <<next_state<<current_state;
       SC_METHOD(AddrMngOutput);
       sensitive <<next_state<<current_state;
       sensitive <<pc_q<<flush_pc_q;

       // PC sampling
       ff_pc = new FliFlop<xlen> ("ff_pc");
       ff_pc  -> d(pc);
       ff_clk -> clk(clk);
       ff_clk -> reset(reset);
       ff_clk -> q(pc_q);

       // Valid sampling
       ff_pc = new FliFlop<1> ("ff_valid");
       ff_pc  -> d(valid_from_pcgen);
       ff_clk -> clk(clk);
       ff_clk -> reset(reset);
       ff_clk -> q(valid_q);

       // Flush PC sampling
       ff_pc = new FliFlop<xlen> ("ff_flus_pc");
       ff_pc  -> d(eu_flush_pc);
       ff_clk -> clk(clk);
       ff_clk -> reset(reset);
       ff_clk -> q(flush_pc_q);

       // Flush Enable sampling
       ff_pc = new FliFlop<1> ("ff_flush_enable");
       ff_pc  -> d(eu_flush_enable);
       ff_clk -> clk(clk);
       ff_clk -> reset(reset);
       ff_clk -> q(flush_enable_q);

   }

   ~Address_Manager()
   {
     delete ff_pc;
     delete ff_valid;
     delete ff_flush_pc;
     delete ff_flush_enable;
   }
}
