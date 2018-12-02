#include "clownfish_ifu_hpi_v0_1.hpp" 

template<int size>
void  FliFlopBit<size>::proc_ff()
{
   q = (reset==1) ? d.read() : 0x0;
}

