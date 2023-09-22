/******************************************************************************
* NDT SYSTEM CONTROL
* Copyright(c) 2018, NewDegreeTechnology, Inc.
*******************************************************************************/

#include "system_control.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_SYS_CONTROL st_sys_ctr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_SCAN_MODE_OPT st_scan_mode_opt = {

	(EM_SCAN_MODE_OPT)DF_SCAN_MODE_SELECT,
	(EM_SCAN_MODE_OPT)EM_SM_ERR,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void system_ctr_init(void)
{
	st_cus_para.scan_rate_poweroff = DF_POWER_OFF;
	st_sys_ctr.period = st_cus_para.scan_rate_high;
	st_sys_ctr.work_ch = 0xff;
}

void cmd_enter_high_speed_period()
{
	if (st_cus_para.scan_rate_high != st_sys_ctr.period
		|| st_cus_para.scan_rate_high != MCU_TM0_REG_VAL
		)
	{
		st_sys_ctr.period = st_cus_para.scan_rate_high;
//		hal_timer_reset(0, st_sys_ctr.period);
	}

	st_scan_mode_opt.enter_low_speed_cnt = 0;  //防止很快又进入idle
}

//if user want to rewrite this function, declare the MACRO SYS_SM_DET_REWRITE and accomplish this func in user file.
#ifndef SYS_SM_DET_REWRITE

void sys_scan_mode_detect(unsigned short algo_out_period[1])
{
	if(st_scan_mode_opt.cur_value != st_scan_mode_opt.pre_value)
	{
		
		switch(st_scan_mode_opt.cur_value)
		{
		case EM_SM_SWITCH:
			st_sys_ctr.period = st_cus_para.scan_rate_high;
			break;
		case EM_SM_HIGH_SPEED:
			st_sys_ctr.period = st_cus_para.scan_rate_high;
			break;
		case EM_SM_LOW_SPEED:
			st_sys_ctr.period = st_cus_para.scan_rate_low;
			break;
		case EM_SM_DEEP_SLEEP:
			st_sys_ctr.period = st_cus_para.scan_rate_deep_sleep;
			break;
		case EM_SM_POWER_OFF:
			st_sys_ctr.period = st_cus_para.scan_rate_poweroff;
		default:
			break;
		}

		st_scan_mode_opt.pre_value = st_scan_mode_opt.cur_value;
		*algo_out_period = st_sys_ctr.period;
	}
}

#endif


void switch_scan_mode_ctr(unsigned short algo_out_period[1])
{
	if (EM_SM_SWITCH == st_scan_mode_opt.cur_value)
	{
		if (0 == st_ff.all_flag && st_scan_mode_opt.enter_low_speed_cnt < ENTER_LOW_SPEED_SCAN_TIME
			&& (st_cus_para.scan_rate_high == st_sys_ctr.period || MCU_TM0_REG_VAL == st_cus_para.scan_rate_high)
			)
		{
		    st_scan_mode_opt.enter_low_speed_cnt += st_sys_ctr.period;
		}
		else
		{
		    st_scan_mode_opt.enter_low_speed_cnt = 0;
		}

		if (st_scan_mode_opt.enter_low_speed_cnt >= ENTER_LOW_SPEED_SCAN_TIME
			&& (st_cus_para.scan_rate_high == st_sys_ctr.period || MCU_TM0_REG_VAL == st_cus_para.scan_rate_high)
			)
		{
			if (st_hal_platform.ap_status & AP_STATUS_POWER_OFF)
			{
				st_sys_ctr.period = st_cus_para.scan_rate_poweroff;
			}
			else
			{
				st_sys_ctr.period = st_cus_para.scan_rate_low;
			}
		}

		if (st_cus_para.scan_rate_high != st_sys_ctr.period
			|| st_cus_para.scan_rate_high != MCU_TM0_REG_VAL
			)
		{
			if (st_fv.max_force[0] > ENTER_HIGH_SPEED_FV_THR
#if (GR_COUNT > 1)
				|| st_fv.max_force[1] > ENTER_HIGH_SPEED_FV_THR
#endif
#if (GR_COUNT > 2)
				|| st_fv.max_force[2] > ENTER_HIGH_SPEED_FV_THR
#endif
				)
			{
				st_sys_ctr.period = st_cus_para.scan_rate_high;
			}
		}
		
		*algo_out_period = st_sys_ctr.period;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void system_period_control(unsigned short algo_out_period[1])
{
	sys_scan_mode_detect(algo_out_period);

	switch_scan_mode_ctr(algo_out_period);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

