#include "clownfish_ifu_address_manager.hpp"

template<int xlen> 
void Address_Manager<xlen>::AddrMngNextState()
{
   if(!reset_n)
     current_state = IDLE;
   else
     current_state = next_state;
}

template<int xlen> 
void Address_Manager<xlen>::AddrMngStateTransfer()
{
   sc_bit back_pressure = ~(iccm_ready_i.read() | ibu_ready_i.read());
   switch(current_state)
   {
      case(IDLE):
      {
        if(valid_from_pcgen_i| eu_flush_enable_i)
           if((PrevillegeSate)(work_mode_i.read()) ==  USER_MODE) 
              next_state = VALID_USER;
           if((PrevillegeSate)(work_mode_i.read()) ==  SUPERVISOR_MODE) 
              next_state = VALID_SUPERVISOR;
           if((PrevillegeSate)(work_mode_i.read()) ==  MACHINE_MODE) 
              next_state = VALID_MACHINE;
        else 
           next_state = IDLE;
        break;
      }
      case(VALID_USER):
      case(VALID_SUPERVISOR):
      case(VALID_VALID_MACHINE):
      {
        if((valid_from_pcgen_i | eu_flush_enable_i) && !back_pressure)
           if((PrevillegeSate)(work_mode_i.read()) ==  USER_MODE) 
              next_state = VALID_USER;
           if((PrevillegeSate)(work_mode_i.read()) ==  SUPERVISOR_MODE) 
              next_state = VALID_SUPERVISOR;
           if((PrevillegeSate)(work_mode_i.read()) ==  MACHINE_MODE) 
              next_state = VALID_MACHINE;
        else if(!(valid_from_pcgen_i | eu_flush_enable_i) && !back_pressure)  
           next_state = IDLE;
        else if((valid_from_pcgen_i | eu_flush_enable_i) && back_pressure)  
           next_state = HOLD;
        break;
      }
      case(HOLD):
      {
        if(eu_flush_enable_i)
           next_state = VALID;
        else if(back_pressure)  
           next_state = HOLD;
        else if(valid_from_pcgen_i && !back_pressure)  
          if((PrevillegeSate)(work_mode.read()) ==  USER_MODE) 
             next_state = VALID_USER;
          if((PrevillegeSate)(work_mode_i.read()) ==  SUPERVISOR_MODE) 
             next_state = VALID_SUPERVISOR;
          if((PrevillegeSate)(work_mode_i.read()) ==  MACHINE_MODE) 
             next_state = VALID_MACHINE;
        else if(!(valid_from_pcgen_i | eu_flush_enable_i) && back_pressure)  
           next_state = IDLE;
        break;
     }
   }
}

template<int xlen> 
void  Address_Manager<xlen>::AddrMngOutput()
{
   if(reset_n)
   {
      iccm_valid_o.write(0);
      ibu_valid_o.write(0);
      address_o.write(IFU_INIT_PC_VALUE);
      ready_to_pc_gen_o.write(0x1);
   }
   else
   {
      switch(next_state)
      {
         case IDLE:
                  {
                    iccm_valid_o.write(0);
                    ibu_valid_o.write(0);
                    address_o.write(pc);
                    ready_to_pc_gen_o.write(0x1);
                    break;
                  }
         case VALID_MACHINE:
                  {  
                    pc = (eu_flush_enable_o == 1)? eu_flush_pc_o.read() : pc;
                    valid = ((pc >= IFU_ICCM_ADDR_START) && (pc<=IFU_ICCM_ADDR_END)) 
                            ? 0x2 : 0x1;
                    iccm_valid_o.write(valid[1]);  
                    ibu_valid_o.write(valid[0]);
                    address_o.write(pc);
                    ready_to_pc_gen_o.write(0x1);
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
                    ready_to_pc_gen.write(0x0);
                    break;
                  }
      }
   }
}
