#pragma once
#include  "system.h"
#include  "stdint.h"
#include  "math.h"
#include  "assert.h"

enum <int> {
   ADDRESS_PHASE = 0,
   B2B_PHASE,
   DATA_PHASE
}  SramState

template<uint32_t dsize, uint32_t asize, uint32_t msize>
SC_MODULE(SRAM)
{
   // input  
   sc_in <sc_bit> ce_n;
   sc_in <sc_bit> clk_i;
   sc_in <sc_bit> we_i;
   sc_in <sc_bit> reset_n;
   sc_in <sc_bv <asize> > address_i; 
   sc_in <sc_bv <dsize> > wdata_i;

   // output
   sc_out <sc_bit> valid_o; 
   sc_out <sc_bv <dsize> > rdata_o 
   sc_out <sc_bit> ready_o; 

   //memory
   char * memory_array;
   sc_signal sc_bv<dsize> wdata;  
   sc_signal sc_bv<dsize> rdata;

   //memroy config 
   char length = dsize/8; 

   //access state
   SramState state;

   //Memory Access Operation
   void MemoryAccess();
   void MemoryStateTransfer();
   void MemoryNextSate();
   void MemoryInit();

   SC_CTOR(SRAM)
   { 
      SC_METHOD(MemoryAccess);
      sensitive<<next_state;
      sensitive_neg<<reset_n;
      sensitive << ce_n << we_i << address_i << wdata_i;

      SC_METHOD(MemoryStateTransfer);
      sensitive<<clk_i;
      sensitive_neg<<reset_n;

      SC_METHOD(MemoryNextState);
      sensitive<<clk_i;
      sensitive_neg<<reset_n;

      SC_METOHD(MemoryInit);
      sensitive_neg<<reset_n; 
   }
}

template<int num, int xlen>
SC_MODULE(ClosedCoupledMemory)
{
   // input 
   sc_in <sc_bit> we_i;
   sc_in <sc_bv <xlen> > address_i;
   sc_in <sc_bv <xlen*num> > wdata_i;
   sc_in <sc_bv <xlen*num/8> > mask_i;
   sc_in <sc_bit> ce_i;

   // output 
   sc_out <sc_bv <xlen*num> > rdata_o;
   sc_out <sc_bv <xlen*num> > mask_o;
   sc_out <sc_bit> valid_o;
   sc_out < >
   sc_out < >
   sc_out < >


   void  
   void 
   void 
   void 

   SC_CTOR(ClosedCoupledMemory)
   {

   }

}