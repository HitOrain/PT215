#include "ndt_algorithm.h"
#include <string.h>


extern s16 model[MAX_SENSOR_COUNT][MAX_MODELCOUNT];

void ndt_algo_param_update(NDT_ALGO_DATA* data)
{
	int i;
	static unsigned char apk_mod_pre;
	static unsigned char factory_mod_pre;
	
	short temp_dac = 0;

	st_common.origin_adc_data[0] = data->package.data_pkg.adc[0];
	st_common.origin_adc_data[1] = data->package.data_pkg.adc[2];
	
	st_common.dac_data[0] = data->ic_operation.dac[0];
	st_common.dac_data[1] = data->ic_operation.dac[2];
	
	for(i=0; i<PHY_CH_COUNT; i++){	
		
		st_common.adc_data[i] = (st_common.origin_adc_data[i] >> ADC_BIT_SHIFT) + ADC_RANG_ADJUST;

		

		data->dynamic_data.selfcheck_log[i] = st_common.adc_data[i];
	
		data->static_data.cali_coeff[i] =  st_common.balance_buf[i] * 1000;

		data->dynamic_data.force_log[i] = st_fv.force_val[i];

		if(0x01==((st_common.dac_data[i]>>10)&0x01)){
			temp_dac = st_common.dac_data[i] & 0x3ff;
			data->static_data.offset[i] = temp_dac;
		
		}else{
			data->static_data.offset[i] = -st_common.dac_data[i];
		}
		
	}

	data->dynamic_data.force_log[PHY_CH_COUNT] = -(data->dynamic_data.force_log[0] + data->dynamic_data.force_log[1]);

	for(i=0; i<2; i++){
		st_tp_info[i].x_point = data->package.state_pkg.tp[i].x;
		st_tp_info[i].y_point = data->package.state_pkg.tp[i].y;
	}
	
	st_common.temprature_cur_aw = (float)data->package.data_pkg.temperature_cur/10;
	st_common.temprature_init = (short)data->static_data.temperature_init[2];
	st_common.temprature_init_bat = (char)data->static_data.temperature_init[0];
	
	st_hal_platform.ap_status = data->package.state_pkg.user_data.game_mode;
	
	for(i=0; i<2; i++) {
		st_config.motor_mod[i] = data->package.state_pkg.user_data.motor_mode[i];
		data->dynamic_data.button_state[i] = st_reg_edge_event.data[i];
	 	st_reg_battery_temp.data[i] = data->static_data.temperature_init[i];
	}

	data->static_data.temperature_real[0] = (unsigned short)(st_common.temprature_real*10);

	data->static_data.temperature_real[2] = (u16)(st_common.temp_coef_buf[0] * CHANNELCAL_DATA_GAINFACTOR);
	data->static_data.temperature_real[3] = (u16)(st_common.temp_coef_buf[1] * CHANNELCAL_DATA_GAINFACTOR);

	data->all_offset_cancel_finish = st_control_flag.all_offset_cancel_finish;

	for(i=0; i<BU_COUNT; i++){
		st_algo_para.button_tri_thr[i] = data->package.state_pkg.user_data.trigger_threshold[i*2];
		st_algo_para.button_rel_thr[i] = data->package.state_pkg.user_data.trigger_threshold[i*2+1];
	}

	if(data->offset_cancel_cnt > 0){
		st_sys_ctr.dis_offset_cancel_cnt = data->offset_cancel_cnt;
		data->offset_cancel_cnt = 0;
	}
	
	if(CMD_OFFSET_CANCEL == data->package.state_pkg.apk_command.factory_mod){
		st_control_flag.all_offset_cancel_finish_pre = false;	//进入工厂模式将动态校准完成标志清除
        st_dac.base_line_limit = BASE_LINE_NARROW;				//补偿目标范围设置为848±100，即执行动态校准
        st_sys_ctr.dis_offset_cancel_cnt = 300000*6;
		st_sys_ctr.is_factory_pattern = CMD_OFFSET_CANCEL;
		st_scan_mode_opt.cur_value = EM_SM_HIGH_SPEED;
		
	}else if(CMD_OFFSET_CANCEL_EXIT == data->package.state_pkg.apk_command.factory_mod
			|| st_sys_ctr.dis_offset_cancel_cnt <=0
	){
		st_sys_ctr.dis_offset_cancel_cnt = 0;
		st_sys_ctr.is_factory_pattern = CMD_OFFSET_CANCEL_EXIT;
		st_scan_mode_opt.cur_value = EM_SM_SWITCH;

		//解决自检后flag置起，无法进行动态校准导致adc数据异常bug
		if(factory_mod_pre != data->package.state_pkg.apk_command.factory_mod){
			st_control_flag.all_offset_cancel_finish_pre = false;	//进入工厂模式将动态校准完成标志清除
        	st_dac.base_line_limit = BASE_LINE_NARROW;				//补偿目标范围设置为848±100，即执行动态校准
        	
	        for (i = 0; i < GR_COUNT; i++){
	            st_ff.sub_flag[i] = em_no_flag;	//清除flag
	            reset_base_from_cur_raw(i);		//复位基线
		    }
		}

	}else if(CMD_OFFSET_CANCEL != st_sys_ctr.is_factory_pattern){
		st_sys_ctr.is_factory_pattern = CMD_OFFSET_CANCEL_EXIT;
	}

	if(data->package.state_pkg.screen_status == 1){		//第一次亮屏IC复位后，先进行一次动态校准
		data->all_offset_cancel_finish = 0;
		st_control_flag.all_offset_cancel_finish_pre = false;	//进入工厂模式将动态校准完成标志清除
		st_dac.base_line_limit = BASE_LINE_NARROW;				//补偿目标范围设置为848±100，即执行动态校准
		
		for (i = 0; i < GR_COUNT; i++){
			st_ff.sub_flag[i] = em_no_flag; //清除flag			
			reset_base_from_cur_raw(i); 	//复位基线

		}
	}

	if(0 == data->package.state_pkg.apk_command.apk_mod && 3 == apk_mod_pre){
		st_control_flag.all_offset_cancel_finish_pre = false;	//进入工厂模式将动态校准完成标志清除
        st_dac.base_line_limit = BASE_LINE_NARROW;				//补偿目标范围设置为848±100，即执行动态校准
	}

	switch (data->package.state_pkg.user_data.refresh_rate)
	{
		case 0:
			st_sys_ctr.period = 4000;
		break;
		case 1:
			st_sys_ctr.period = 16;
		break;
		default:
			st_sys_ctr.period = 6;
	}
	
	for(int i=0; i<14*12; i++){
		model[0][i] = data->dynamic_data.cali_coeff_pkg[0][i];
	}

	for(int i=0; i<15*12; i++){
		model[1][i] = data->dynamic_data.cali_coeff_pkg[1][i];
	}
	
	factory_mod_pre = data->package.state_pkg.apk_command.factory_mod;
	apk_mod_pre = data->package.state_pkg.apk_command.apk_mod;

#if 0
	printf("init = %d, init_bat%d\n", st_common.temprature_init, st_common.temprature_init_bat);

	printf("\n\n\n\n\n\n\n\n\n\n");
	
	for(int i=0; i<14*12; i++){
		printf("%d ", model[0][i]);
		if(0==i%11 && i!=0){
			printf("\n");
		}
	}

	printf("\n\n\n\n\n\n\n\n\n\n");

	for(int i=0; i<15*12; i++){
		printf("%d ", model[1][i]);
		if(0==i%11 && i!=0){
			printf("\n");
		}
	}
#endif

}

int cali_log_update(void *data)
{
	return output_demo_apk_data(data);
}

int effect_log_update(void *data)
{
	return output_user_debug_data(data);
}

void on_off_status_update(unsigned char on_off_status[18], unsigned char const on_off_status_all[17])
{
	short temp_status = 0;
	short *status = (short*)(&on_off_status[0]);

	//ch0 P
	on_off_status[2] = on_off_status_all[1];
	on_off_status[3] = on_off_status_all[2];

	//ch0 N
	on_off_status[4] = on_off_status_all[3];
	on_off_status[5] = on_off_status_all[4];

	//ch2 P
	on_off_status[6] = on_off_status_all[9];
	on_off_status[7] = on_off_status_all[10];

	//ch2 N
	on_off_status[8] = on_off_status_all[11];
	on_off_status[9] = on_off_status_all[12];

	for(int i=0; i<PHY_CH_COUNT*2; i++){
		if((0 != status[i+1]) || (status[i+1]<92 && status[i+1]>108)){
			temp_status |=0x01<<i;
		}
	}

	for(int i=0; i<PHY_CH_COUNT; i++){
		if(((temp_status>>i*2)&0x0003) == 0x03){
			status[0]|=(0x01<<i);
		}
	}

	status[0]=~status[0];

}


extern u16 skip_dynamic_calibration;


void algo_debug_prepare(NDT_ALGO_DATA* data)
{

	sprintf(data->dynamic_data.algo_debug_info, 
	
		"\t%d.%d.%d.%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
		
		st_reg_fw_ver.data[0], 
		st_reg_fw_ver.data[1], 
		st_reg_fw_ver.data[2], 
		st_reg_fw_ver.data[3],
		st_reg_edge_event.data[1],
		st_ff.all_flag, 
		st_tf.all_flag,
		st_common.origin_adc_data[0],
		st_common.origin_adc_data[1],
		st_common.adc_data[0],
		st_common.adc_data[1],
		st_common.dac_data[0],
		st_common.dac_data[1],
		st_raw.current_buf[0],
		st_raw.current_buf[1],
		st_base.base[0],
		st_base.base[1],
		st_fv.force_val[0],
		st_fv.force_val[1],
		st_sys_ctr.is_factory_pattern,
		st_sys_ctr.dis_offset_cancel_cnt,
		st_dac.total_cali_times,
		st_sys_ctr.period,
		st_scan_mode_opt.cur_value,
		skip_dynamic_calibration,
		st_tp_info[0].x_point,
		st_tp_info[0].y_point,
		st_tp_info[1].x_point,
		st_tp_info[1].y_point,
		st_hal_platform.ap_status
	);
	
	printf("%d.%d.%d.%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		
		st_reg_fw_ver.data[0], 
		st_reg_fw_ver.data[1], 
		st_reg_fw_ver.data[2], 
		st_reg_fw_ver.data[3],
		st_reg_edge_event.data[1],
		st_ff.all_flag, 
		st_tf.all_flag,
		st_common.origin_adc_data[0],
		st_common.origin_adc_data[1],
		st_common.adc_data[0],
		st_common.adc_data[1],
		st_common.dac_data[0],
		st_common.dac_data[1],
		st_raw.current_buf[0],
		st_raw.current_buf[1],
		st_base.base[0],
		st_base.base[1],
		st_fv.force_val[0],
		st_fv.force_val[1],
		st_sys_ctr.is_factory_pattern,
		st_sys_ctr.dis_offset_cancel_cnt,
		st_dac.total_cali_times,
		st_sys_ctr.period,
		st_scan_mode_opt.cur_value,
		skip_dynamic_calibration,
		st_tp_info[0].x_point,
		st_tp_info[0].y_point,
		st_tp_info[1].x_point,
		st_tp_info[1].y_point,
		st_hal_platform.ap_status);
}

void ndt_algo_proc(NDT_ALGO_DATA*      data)
{
		
 	hal_heart_refresh();

	alg_dbg_reset();
	
	cus_monitor();

	ndt_algorithm_process(st_common.adc_data);

//	algo_debug_prepare(data);

	hal_event_detect();
	
	data->dynamic_data.button_state[1] = st_reg_edge_event.data[1];
}

void ndt_init(NDT_ALGO_DATA* data)
{
	memcpy(&data->static_data.version_id, &st_reg_fw_ver.data, 4);
	
	memcpy(&data->static_data.adc_conv_factor, &st_reg_adc_2_uvolt_coef.data, 3);
	memcpy(&data->static_data.dac_conv_factor, &st_reg_dac_2_uvolt_coef.data, 2);

	for(int i=0; i<BU_COUNT; i++){
		data->package.state_pkg.user_data.trigger_threshold[i*2]   = st_algo_para.button_tri_thr[i];
		data->package.state_pkg.user_data.trigger_threshold[i*2+1] = st_algo_para.button_rel_thr[i];
	}

	data->package.state_pkg.user_data.refresh_rate = st_scan_mode_opt.cur_value;

	data->static_data.temperature_init[0] = st_common.temprature_init_bat;
	data->static_data.temperature_init[1] = 0xff;

	data->static_data.temperature_init[2] = (unsigned char)st_common.temprature_init;
	data->static_data.temperature_init[3] = 0xff;

	data->dynamic_data.temperature_init_bat = st_common.temprature_init_bat;
	data->algo_out_period = DF_HIGH_SPEED;

	data->package.state_pkg.apk_command.factory_mod = 0;

	data->package.state_pkg.user_data.refresh_rate = 0x01;

	
	hal_platform_init();
	
	hal_flash_para_init();
	
	ndt_algorithm_init();

	system_ctr_init();
	
	cus_flash_para_reinit();
}










