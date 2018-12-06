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
       sensitive_pos <<clk
       sensitive_neg <<reset;

       //state transfer combine-logic
       SC_METHOD(AddrMngAddMngNextSate);
       sensitive <<valid_from_pcgen;
       sensitive <<flush_enable;
       sensitive <<iccm_ready<<ibu_ready;
       sensitive <<current_state;

       SC_METHOD(AddrMngOutput);
       sensitive_pos <<clk;
       sensitive_neg <<reset

       //SC_METHOD(AddMngDebugThread);
       //sensitive <<

   }

   ~Address_Manager()
   {
   }
}
