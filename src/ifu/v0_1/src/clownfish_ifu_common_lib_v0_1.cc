#include "clownfish_ifu_common_lib.hpp"

template<uint32_t dsize, uint32_t asize, uint32_t msize>
void SRAM<dsize, aisze, msize>::MemoryInit()
{
  if(!reset_n)
  { 
    
    if(memory_array != NULL)
      free(memory_array);
    memory_array = (char *)malloc(sizof(char)*pow(2, msize));
    state =  ADDRESS_PHASE;
  }
}

void SRAM<dsize, aisze, msize>::MemoryNextState()
{
   if(!reset_n){ 
     current_state = ADDRESS_PHASE;
     next_address = 0x0;
     next_wdata = 0x0;
     next_we = 0x0;
   }
   else
   { 
     current_state = next_state;
     current_address = next_address;
     current_we = next_we;
     current_wdata = next_wdata;
   }
}

void SRAM<dsize, aisze, msize>::MemoryStateTransfer()
{

  switch(current_state)
  {
    case(ADDRESS_PHASE):
    {
       if(!ce_n)
         next_state = DATA_PHASE;
    }
    case(DATA_PHASE):
    {
      //B2B phase
      if(!ce_n)
         next_state = DATA_PHASE;
      else
        next_state = ADDRESS_PHASE;
    }
  }

}

void SRAM<dsize, aisze, msize>::MemoryAccess()
{
  // prevent memory array not initial 
  valid_o.write(0x0);
  ready_o.write(0x0);
  assert(memory_array!=NULL);
  sc_uint address;
  address = address_i.read();

  if(!reset_n)
  {
    valid_o.write(0x0);
    ready_o.write(0x1);
    rdata_o.write(0x0);
  }
  else
  {
     // write operation
    switch(next_state)
    {
      case(ADDRESS_PHASE):
      {
         next_we = we_i.read();
         if(next_we)
           next_wdata = wdata_i.read();
         next_address = address_i.read();
         ready_o.write(0x1);
         valid_o.write(0x0);
      }
      case(DATA_PHASE):
      {
         if(next_we)
         {
             sc_bv<dsize> wdata;  
             wdata = wdata.read();
             for(uint32 i=0; i<length; i++)
             {
                 if(address < pow(2, disize))
                   memory_array[address] =  wdata.range(8*(i+1), 8*i);
                 else
                   printf("INFO : Warnning, address : %x out of range", address);
                 address++;
             }
         }
         // read operation
         else
         {
             sc_bv<dsize> rdata;
             for(uint32 i=0; i<length; i++)
             {
                 if(address < pow(2, disize))
                     wdata.range(8*(i+1), 8*i) = memory_array[address];
                 else
                   wdata.range(8*(i+1), 8*i) = 0x00;
                 address++;
             }

         }
      }
    }
    valid_o.write(0x1);
    ready_o.write(0x1);
  }
    
}
