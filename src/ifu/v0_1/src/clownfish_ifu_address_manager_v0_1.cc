#include "clownfish_ifu_address_manager.hpp"

template<int xlen> 
void Address_Manager<xlen>::AddrMngStateTransfer()
{
   if(!reset)
     state = IDLE;
   else
   {
     sc_bit back_pressure = ~(iccm_ready.read() | ibu_ready.read());
     switch(state)
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

   if(reset)
   {
      iccm_valid.write(0);
      ibu_valid.write(0);
      address.write(IFU_INIT_PC_VALUE);
      ready_to_pc_gen(0x0);
   }
   else
   {
      switch(state)
      {
         case IDLE:
                  {
                    iccm_valid.write(0);
                    ibu_valid.write(0);
                    address.write(IFU_INIT_PC_VALUE);
                    ready_to_pc_gen(0x0);
                    break;
                  }
         case VALID:
                  {  
                    switch((PrevillegeSate)(work_mode_q.read())
                    {
                       case USER_MODE:
                           {
       
                              break;
                           }
                       case HYPERVISOR_MODE:
                           {

                              break;
                           }
                       case SUPERVISOR_MODE: 
                           {

                              break;
                           }
                       case MACHINE_MODE:
                           {  
                              sc_bv[2] valid;
                              sc_bcp[xlen] pc;
                              pc = (eu_flush_enable == 1)? flush_pc_q.read() 
                                                           : pc_q.read();
                              valid = 
                              iccm_valid  
                              break;
                           }
                    }
                    iccm_valid.write(0);
                    ibu_valid.write(0);
                    address.write(IFU_INIT_PC_VALUE);
                    ready_to_pc_gen(0x0);

                  }
         case HOLD:
                  {
                    iccm_valid.write(0);
                    ibu_valid.write(0);
                    address.write(IFU_INIT_PC_VALUE);
                    ready_to_pc_gen(0x0);
                  }
      }
   }

}
