#pragma once
#include "systemc.h"
#include "clownfish_ifu_hpi.hpp"
#include "clownfish_ifu_parameters.hpp"
#include <stdint.h>


enum <int>{
  IDLE  = 0,
  HOLD, 
  VALID_MACHINE,
  VALID_USER,
  VALID_SUPERVISIOR,
  VALID_HYPERVISOR

} AddrMngSate;

template<int xlen>
SC_MODULE(Address_Manager)
{
    // input 
   sc_in <sc_bit> clk_i, reset_n;
   sc_in <sc_bv <xlen> > pc_i;
   sc_in <sc_bit>  valid_from_pcgen_i;
   sc_in <sc_bit> iccm_ready_i, ibu_ready_i  
   sc_in <sc_bit> eu_flush_enable_i;
   sc_in <sc_bv <xlen> > eu_flush_pc_i;
   sc_in <sc_bv <2> > work_mode_i;

   // output
   sc_out <sc_bit> iccm_valid_o; ibu_valid_o;
   sc_out <sc_bv <xlen> > address_o;
   sc_out <sc_bit> ready_to_pcgen_o;

   //inter signals 
   //FSM
   AddrMngSate next_state, current_state;

   //output
   sc_bv[2] valid;
   sc_bv[xlen] pc;

   void AddrMngStateTransfer();
   void AddMngNextState();
   void AddrMngOutput();
   //void AddMngDebugThread();

   SC_CTOR(Address_Manager)
   {
       //state transfer sync-logic
       SC_METHOD(AddrMngStateTransfer);
       sensitive_pos <<clk_i
       sensitive_neg <<reset_n;

       //state transfer combine-logic
       SC_METHOD(AddrMngAddMngNextSate);
       sensitive <<valid_from_pcgen_i;
       sensitive <<eu_flush_enable_i;
       sensitive <<iccm_ready_i<<ibu_ready_i;
       sensitive <<current_state;

       SC_METHOD(AddrMngOutput);
       sensitive_pos <<clk_i;
       sensitive_neg <<reset_n

       //SC_METHOD(AddMngDebugThread);
       //sensitive <}<
   }

   ~Address_Manager()
   {
   }
}
