#include "clownfish_ifu_hpi.hpp" 

template<int size>
void  FliFlopBit<size>::proc_ff()
{
   q = (reset==1) ? d.read() : 0x0;
}

