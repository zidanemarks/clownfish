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
   sc_in <sc_bv <2> > work_mode;

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
   sc_signal <sc_bv <2> > work_mode_q;

   void AddrMngStateTransfer();
   void AddrMngOutput();
   void PrevillegeChange();
   void DebugThread();

   FliFlop<xlen> *ff_pc;
   FliFlop<1> *ff_valid;
   FliFlop<xlen> *ff_flush_pc;
   FliFlop<1> *ff_flush_enable;
   FliFlop<2> *ff_work_mode;

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

      SC_METHOD(PrevillegeChange)

       // PC sampling
       ff_pc = new FliFlop<xlen> ("ff_pc");
       ff_pc -> d(pc);
       ff_pc -> clk(clk);
       ff_pc -> reset(reset);
       ff_pc -> q(pc_q);

       // Valid sampling
       ff_valid = new FliFlop<1> ("ff_valid");
       ff_valid -> d(valid_from_pcgen);
       ff_valid -> clk(clk);
       ff_valid -> reset(reset);
       ff_valid -> q(valid_q);

       // Flush PC sampling
       ff_flush_pc = new FliFlop<xlen> ("ff_flus_pc");
       ff_flush_pc -> d(eu_flush_pc);
       ff_flush_pc -> clk(clk);
       ff_flush_pc -> reset(reset);
       ff_flush_pc -> q(flush_pc_q);

       // Flush Enable sampling
       ff_flush_enable = new FliFlop<1> ("ff_flush_enable");
       ff_flush_enable -> d(eu_flush_enable);
       ff_flush_enable -> clk(clk);
       ff_flush_enable -> reset(reset);
       ff_flush_enable -> q(flush_enable_q);

       // Work Mode Sampling
       ff_work_mode = new FliFlop<2> ("ff_work_mode");
       ff_work_mode -> d(work_mode);
       ff_work_mode -> clk(clk);
       ff_work_mode -> reset(reset);
       ff_work_mode -> q(work_mode_q);

   }

   ~Address_Manager()
   {
     delete ff_pc;
     delete ff_valid;
     delete ff_flush_pc;
     delete ff_flush_enable;
     delete ff_work_mode;
   }
}
