#include "clownfish_ifu_address_manager_v0_1.hpp"

template<int xlen> 
void Address_Manager<xlen>::AddrMngStateTransfer()
{
   if(!reset)
     state = IDLE;
   else
   {
     sc_bit back_pressure = ~(iccm_ready | ibu_ready);
     switch(current_state)
     {
        case(IDLE):
        {
          if(valid_q | flush_en_q)
             state = VALID;
          else 
             state = IDLE;
        }
        case(VALID):
        {
          if((valid_q | flush_en_q) && !back_pressure)
             state = VALID;
          else if(!(valid_q | flush_en_q) && !back_pressure)  
             state = IDLE;
          else if((valid_q | flush_en_q) && back_pressure)  
             state = HOLD;
        }
        case(HOLD):
        {
          if(flush_en_q)
             state = VALID;
          else if(back_pressure)  
             state = HOLD;
          else if(valid_q && !back_pressure)  
             state = VALID;
          else if(!(valid_q | flush_en_q) && back_pressure)  
             state = IDLE;
       }
     }
   }
}


template<int xlen> 
void  Address_Manager<xlen>::AddrMngOutput()
{

}
