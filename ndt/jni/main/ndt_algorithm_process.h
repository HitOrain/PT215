/*************************************************
* NDT RAW DATA
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __NDT_ALGO_PROCESS_H
#define __NDT_ALGO_PROCESS_H

#include "aw8686x_data_process.h"

extern void ndt_offset_cancel(char all_offset_cancel_finish[1], short dac[4]);
extern void ndt_init(NDT_ALGO_DATA* data);
extern void ndt_algo_proc(NDT_ALGO_DATA* data);
extern void ndt_algo_param_update(NDT_ALGO_DATA* data);
extern void on_off_status_update(unsigned char on_off_status[18], unsigned char const on_off_status_all[17]);
extern void system_period_control(unsigned short algo_out_period[1]);
extern void interaction_interface(NDT_ALGO_DATA* data);

////////////////////////////////////////////////////////////////////////////////////////////////

#endif	/* __NDT_ALGO_PROCESS_H */
