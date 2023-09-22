#ifndef __FLASH_RW__H__
#define __FLASH_RW__H__

#include "hal_ndt_common.h"

#ifdef AW_8686X

#include "compile_option.h"

	 
#define PROFILE_BLOCK_SEC_START			(48 << 1)
#define PROFILE_BLOCK_SEC_SIZE			(1 << 9)

#define BLOCK_MIN						0
#define BLOCK_MAX						21

//////////////////////block location define//////////////////
#define CALIBRATION_BLOCK               21

#define MODEL1_2_BLOCK                  20
#define MODEL3_4_BLOCK                  19
#define MODEL5_6_BLOCK                  18
#define MODEL7_8_BLOCK                  17
#define MODEL9_10_BLOCK                 16
#define MODEL11_12_BLOCK                15
#define MODEL13_14_BLOCK                14
#define MODEL15_16_BLOCK                13

#define FW_CONFIG_BLOCK                 12
#define AP_TEST_BLOCK                   11

#define MODEL_BLOCK_MAX                 8
#define ONE_MODE_MAX_SIZE               256

#ifndef CONST_LARGE_MODEL_LEN
#define CONST_LARGE_MODEL_LEN           250
#endif
//////////////////////block location define//////////////////



extern u8 temp_buff[];

#endif
#endif
