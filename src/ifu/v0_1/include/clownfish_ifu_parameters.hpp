//this files is animate the ifu registers and OTP/Efuse
#pragma once
#include <stdint.h>

// a register which have ifu pc init value
const uint32_t IFU_INIT_PC_VALUE = 0x8000000;
// define ICCM size = 128K
const uint32_t IFU_ICCM_ADDR_START = 0x00000000;
const uint32_t IFU_ICCM_ADDR_END   = 0x00020000-0x4;

//working mode 
enum <int>{
  USER_MODE  = 0,
  HYPERVISOR_MODE,
  SUPERVISOR_MODE, 
  MACHINE_MODE
} PrevillegeSate;

uint32_t IFU_PREVILLEGE_MODE = MACHINE_MODE;