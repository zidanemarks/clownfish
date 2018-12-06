#include "clownfish_ifu_address_manager.hpp"

template<int xlen> 
void Address_Manager<xlen>::AddrMngNextState()
{
   if(!reset)
     current_state = IDLE;
   else
     current_state = next_state;
}

template<int xlen> 
void Address_Manager<xlen>::AddrMngStateTransfer()
{
   sc_bit back_pressure = ~(iccm_ready.read() | ibu_ready.read());
   switch(current_state)
   {
      case(IDLE):
      {
        if(valid_from_pcgen| flush_en)
           if((PrevillegeSate)(work_mode.read()) ==  USER_MODE) 
              next_state = VALID_USER;
           if((PrevillegeSate)(work_mode.read()) ==  SUPERVISOR_MODE) 
              next_state = VALID_SUPERVISOR;
           if((PrevillegeSate)(work_mode.read()) ==  MACHINE_MODE) 
              next_state = VALID_MACHINE;
        else 
           state = IDLE;
        break;
      }
      case(VALID_USER):
      case(VALID_SUPERVISOR):
      case(VALID_VALID_MACHINE):
      {
        if((valid_q | flush_en_q) && !back_pressure)
           if((PrevillegeSate)(work_mode.read()) ==  USER_MODE) 
              next_state = VALID_USER;
           if((PrevillegeSate)(work_mode.read()) ==  SUPERVISOR_MODE) 
              next_state = VALID_SUPERVISOR;
           if((PrevillegeSate)(work_mode.read()) ==  MACHINE_MODE) 
              next_state = VALID_MACHINE;
        else if(!(valid_q | flush_en_q) && !back_pressure)  
           state = IDLE;
        else if((valid_q | flush_en_q) && back_pressure)  
           state = HOLD;
        break;
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
        break;
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
      ready_to_pc_gen.write(0x1);
   }
   else
   {
      switch(next_state)
      {
         case IDLE:
                  {
                    iccm_valid.write(0);
                    ibu_valid.write(0);
                    address.write(pc);
                    ready_to_pc_gen.ready(0x1);
                    break;
                  }
         case VALID_MACHINE:
                  {  
                    pc = (eu_flush_enable == 1)? flush_pc.read() :  pc;
                    valid = ((pc >= IFU_ICCM_ADDR_START) && (pc<=IFU_ICCM_ADDR_END)) 
                            ? 0x2 : 0x1;
                    iccm_valid.write(valid[1]);  
                    ibu_valid.write(valid[0]);
                    address.write(pc);
                    ready_to_pc_gen(0x1);
                    break;
                  }
         case VALID_USER:
         {}
         case VALID_SUPERVISOR:
         {}
         case HOLD:
                  {
                    iccm_valid.write(valid[1]);
                    ibu_valid.write(valid[0]);
                    address.write(pc);
                    ready_to_pc_gen(0x0);
                  }
      }
   }
}
