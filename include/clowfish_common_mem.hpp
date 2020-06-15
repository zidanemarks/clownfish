#pragma once
#include "systemc.h"
#include "stdint.h"



template<uint32_t dsize, uinit32_t asize, uint32_t msize>
SC_MODULE(Memory_SinPort)
{
    //input 
    sc_in <sc_bit> ce_n;
    sc_in <sc_bit> clk_i;
    sc_in <sc_bit> we_i;
    sc_in <sc_bit> reset_n;
    sc_in <sc_bv <asize> > address_i; 
    sc_in <sc_bv <dsize> > wdata_i;

    // output
    sc_out <sc_bv <dsize> > rdata_o 

    //memory
    char * memory;

    SC_CTOR(Memory_SinPort)
    {
        SC_METHOD(MemoryAccess);
        sen
    }



}
