#include  "system.h"

template<int dsize, int msize>
SC_MODULE(SRAM)
{
   // input  
   sc_in <sc_bit> ce_i;
   sc_in <sc_bit> clk_i;
   sc_in < > address_i 
   sc_in <dsize> wdata_i

   // output
   sc_out <> 
   sc_out <> 
   sc_out <> 

   SC_CTOR(ClosedCoupledMemory)
   {

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