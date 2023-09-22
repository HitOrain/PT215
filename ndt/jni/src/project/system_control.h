/*************************************************
* NDT SYSTEM CONTROL
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __SYSTEM_CONTROL_H
#define __SYSTEM_CONTROL_H

#include "compile_option.h"

#include "algorithm_init.h"

#include "hal_ndt_common.h"

#include "aw8686x_data_process.h"
//////////////////////////////////////////////////////////////////////////////////////////

typedef enum{
	EM_CMD_SW_RESET = 0x01,
	
} EM_CMD_OPT;

extern EM_CMD_OPT em_cmd_opt;

//////////////////////////////////////////////////////////////////////////////////////////

typedef enum{
	EM_SM_SWITCH = 0x01,
	EM_SM_HIGH_SPEED,
	EM_SM_LOW_SPEED,
	EM_SM_DEEP_SLEEP,
	EM_SM_POWER_OFF,
	EM_SM_USER_DEFINED1,
	EM_SM_USER_DEFINED2,
	EM_SM_ERR,
} EM_SCAN_MODE_OPT;

////////////////////////////////////////////////////////////

typedef struct{

	EM_SCAN_MODE_OPT cur_value;
	EM_SCAN_MODE_OPT pre_value;
	
	u16 enter_low_speed_cnt;
	
} ST_SCAN_MODE_OPT;

extern ST_SCAN_MODE_OPT st_scan_mode_opt;

#ifndef ENTER_LOW_SPEED_SCAN_TIME
#define ENTER_LOW_SPEED_SCAN_TIME 5000
#endif
#ifndef ENTER_HIGH_SPEED_FV_THR
#define ENTER_HIGH_SPEED_FV_THR 50
#endif



//////////////////////////////////////////////////////////////////////////////////////////
typedef struct{
	u16 period;
	u16 work_ch;
	u16 is_factory_pattern;
	s16 dis_report_int_cnt;
	s16 dis_report_button_cnt;
	s16 io_toggle_cnt;
	s32 dis_offset_cancel_cnt;
}ST_SYS_CONTROL;

extern ST_SYS_CONTROL st_sys_ctr;
extern void system_control(unsigned short algo_out_period[1]);
extern void system_ctr_init(void);
extern void cmd_enter_high_speed_period(void);
//////////////////////////////////////////////////////////////////////////////////////////

#endif  /* __SYSTEM_CONTROL_H */
