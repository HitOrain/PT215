/******************************************************************************
* NDT REG TABLE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
*******************************************************************************/

#include "reg_table.h"
#include "user_interface.h"
#include "hal_ndt_common.h"
#include "algorithm_init.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rw_null_reg(void)
{
	//st_iic.data_ptr = &(st_reg_iic_test.data[0]);
	st_iic.r_count = 0;
	st_iic.w_count = 0;
}

void rw_iic_test(void)
{
	st_iic.data_ptr = &(st_reg_iic_test.data[0]);
	st_iic.r_count = st_reg_iic_test.r_count;
	st_iic.w_count = st_reg_iic_test.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_handshake(void)
{
	st_iic.data_ptr = &(st_reg_handshake.data[0]);
	st_iic.r_count = st_reg_handshake.r_count;
	st_iic.w_count = st_reg_handshake.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_cmd(void)
{
	st_iic.data_ptr = &(st_reg_cmd.data[0]);
	st_iic.r_count = st_reg_cmd.r_count;
	st_iic.w_count = st_reg_cmd.w_count;
}

void rw_heart_beat(void)
{
	st_iic.data_ptr = &(st_reg_heart.data[0]);
	st_iic.r_count = st_reg_heart.r_count;
	st_iic.w_count = st_reg_heart.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_count_info(void)
{
	st_iic.data_ptr = &(st_reg_count_info.data[0]);
	st_iic.r_count = st_reg_count_info.r_count;
	st_iic.w_count = st_reg_count_info.w_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_mcu_id(void)
{
	st_iic.data_ptr = &(st_reg_mcu_id.data[0]);
	st_iic.r_count = st_reg_mcu_id.r_count;
	st_iic.w_count = st_reg_mcu_id.w_count;
}

void read_rst_src_id(void)
{
	st_iic.data_ptr = &(st_reg_rst_src.data[0]);
	st_iic.r_count = st_reg_rst_src.r_count;
	st_iic.w_count = st_reg_rst_src.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_project_id(void)
{
	st_iic.data_ptr = &(st_reg_project_id.data[0]);
	st_iic.r_count = st_reg_project_id.r_count;
	st_iic.w_count = st_reg_project_id.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_fw_ver(void)
{
	st_iic.data_ptr = &(st_reg_fw_ver.data[0]);
	st_iic.r_count = st_reg_fw_ver.r_count;
	st_iic.w_count = st_reg_fw_ver.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void read_screen_info(void)
{
#if defined(SCREEN_INFO_SUPPORT)  
    uint32_t tmp;
    uint8_t p_tmp=0;
    uint16_t crc_vlaue;
    
    tmp =(uint32_t)(1000*m_screen_info.x_size_mm);
    reg_screen_info.data[p_tmp++] =tmp>>0;
    reg_screen_info.data[p_tmp++] =tmp>>8;
    reg_screen_info.data[p_tmp++] =tmp>>16;
    reg_screen_info.data[p_tmp++] =tmp>>24;
    
    tmp =(uint32_t)(1000*m_screen_info.y_size_mm);
    reg_screen_info.data[p_tmp++] =tmp>>0;
    reg_screen_info.data[p_tmp++] =tmp>>8;
    reg_screen_info.data[p_tmp++] =tmp>>16;
    reg_screen_info.data[p_tmp++] =tmp>>24;
    
    tmp =m_screen_info.x_pixel_num;
    reg_screen_info.data[p_tmp++] =tmp>>0;
    reg_screen_info.data[p_tmp++] =tmp>>8;
    reg_screen_info.data[p_tmp++] =tmp>>16;
    reg_screen_info.data[p_tmp++] =tmp>>24;
    
    tmp =m_screen_info.y_pixel_num;
    reg_screen_info.data[p_tmp++] =tmp>>0;
    reg_screen_info.data[p_tmp++] =tmp>>8;
    reg_screen_info.data[p_tmp++] =tmp>>16;
    reg_screen_info.data[p_tmp++] =tmp>>24;
    
    crc_vlaue = 0 - calc_checksum_s16((u8 *)reg_screen_info.data, p_tmp);

    reg_screen_info.data[p_tmp++] =crc_vlaue>>0;
    reg_screen_info.data[p_tmp++] =crc_vlaue>>8;
    
	st_iic.data_ptr = &(reg_screen_info.data[0]);
	st_iic.r_count = reg_screen_info.r_count;
	st_iic.w_count = reg_screen_info.w_count;
#endif //
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_afe_gain(void)
{
	st_iic.data_ptr = &(st_reg_afe_gain.data[0]);
	st_iic.r_count = st_reg_afe_gain.r_count;
	st_iic.w_count = st_reg_afe_gain.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_dac_value(void)
{
	st_iic.data_ptr = &(st_reg_dac_value.data[0]);
	st_iic.r_count = st_reg_dac_value.r_count;
	st_iic.w_count = st_reg_dac_value.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_adc_2_uvolt_coef(void)
{
	st_iic.data_ptr = &(st_reg_adc_2_uvolt_coef.data[0]);
	st_iic.r_count = st_reg_adc_2_uvolt_coef.r_count;
	st_iic.w_count = st_reg_adc_2_uvolt_coef.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_dac_2_uvolt_coef(void)
{
	st_iic.data_ptr = &(st_reg_dac_2_uvolt_coef.data[0]);
	st_iic.r_count = st_reg_dac_2_uvolt_coef.r_count;
	st_iic.w_count = st_reg_dac_2_uvolt_coef.w_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_group_map_info(void)
{
	st_iic.data_ptr = &(st_reg_group_map_info.data[0]);
	st_iic.r_count = st_reg_group_map_info.r_count;
	st_iic.w_count = st_reg_group_map_info.w_count;
}

void read_noise_thr(void)
{
	st_iic.data_ptr = &(st_reg_noise_thr.data[0]);
	st_iic.r_count = st_reg_noise_thr.r_count;
	st_iic.w_count = st_reg_noise_thr.w_count;
}

void read_chip_noise_thr(void)
{
	st_iic.data_ptr = &(st_reg_chip_noise_thr.data[0]);
	st_iic.r_count = st_reg_chip_noise_thr.r_count;
	st_iic.w_count = st_reg_chip_noise_thr.w_count;
}

void read_offset_thr(void)
{
	st_iic.data_ptr = &(st_reg_offset_thr.data[0]);
	st_iic.r_count = st_reg_offset_thr.r_count;
	st_iic.w_count = st_reg_offset_thr.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_balance_info(void)
{
	st_iic.data_ptr = &(st_reg_balance_info.data[0]);
	st_iic.r_count = st_reg_balance_info.r_count;
	st_iic.w_count = st_reg_balance_info.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_temp_info(void)
{
	st_iic.data_ptr = &(st_reg_temp_info.data[0]);
	st_iic.r_count = st_reg_temp_info.r_count;
	st_iic.w_count = st_reg_temp_info.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_channel_gap(void)
{
	st_iic.data_ptr = &(st_reg_channel_gap.data[0]);
	st_iic.r_count = st_reg_channel_gap.r_count;
	st_iic.w_count = st_reg_channel_gap.w_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void read_ch_conn_status(void)
{
	st_iic.data_ptr = &(st_reg_ch_conn_status.data[0]);
	st_iic.r_count = st_reg_ch_conn_status.r_count;
	st_iic.w_count = st_reg_ch_conn_status.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void read_gpio_status(void)
{
	st_iic.data_ptr = &(st_reg_gpio_status.data[0]);
	st_iic.r_count = st_reg_gpio_status.r_count;
	st_iic.w_count = st_reg_gpio_status.w_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rw_time_out(void)
{
	st_iic.data_ptr = &(st_reg_time_out.data[0]);
	st_iic.r_count = st_reg_time_out.r_count;
	st_iic.w_count = st_reg_time_out.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_battery_temp(void)
{
	st_iic.data_ptr = &(st_reg_battery_temp.data[0]);
	st_iic.r_count = st_reg_battery_temp.r_count;
	st_iic.w_count = st_reg_battery_temp.w_count;
}


void rw_ap_status(void)
{
	st_iic.data_ptr = &(st_reg_ap_status.data[0]);
	st_iic.r_count = st_reg_ap_status.r_count;
	st_iic.w_count = st_reg_ap_status.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////


void rw_scan_mode_switch(void)
{
	st_iic.data_ptr = &(st_reg_scan_mode_switch.data[0]);
	st_iic.r_count = st_reg_scan_mode_switch.r_count;
	st_iic.w_count = st_reg_scan_mode_switch.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_fun_switch(void)
{
	st_iic.data_ptr = &(st_reg_fun_switch.data[0]);
	st_iic.r_count = st_reg_fun_switch.r_count;
	st_iic.w_count = st_reg_fun_switch.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_button_force_thr(void)
{
	st_iic.data_ptr = &(st_reg_button_force_thr.data[0]);
	st_iic.r_count = st_reg_button_force_thr.r_count;
	st_iic.w_count = st_reg_button_force_thr.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_button_pos(void)
{
	st_iic.data_ptr = &(st_reg_button_pos.data[0]);
	st_iic.r_count = st_reg_button_pos.r_count;
	st_iic.w_count = st_reg_button_pos.w_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void rw_slide_force_thr(void)
{
	st_iic.data_ptr = &(st_reg_slide_force_thr.data[0]);
	st_iic.r_count = st_reg_slide_force_thr.r_count;
	st_iic.w_count = st_reg_slide_force_thr.w_count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rw_edge_event(void)
{
	st_iic.data_ptr = &(st_reg_edge_event.data[0]);
	st_iic.r_count = st_reg_edge_event.r_count;
	st_iic.w_count = st_reg_edge_event.w_count;
}

void rw_cus_def_reg1(void)
{
	st_iic.data_ptr = &(st_reg_cus_def_reg1.data[0]);
	st_iic.r_count = st_reg_cus_def_reg1.r_count;
	st_iic.w_count = st_reg_cus_def_reg1.w_count;
}
void rw_cus_def_reg2(void)
{
	st_iic.data_ptr = &(st_reg_cus_def_reg2.data[0]);
	st_iic.r_count = st_reg_cus_def_reg2.r_count;
	st_iic.w_count = st_reg_cus_def_reg2.w_count;
}
void rw_cus_def_reg3(void)
{
	st_iic.data_ptr = &(st_reg_cus_def_reg3.data[0]);
	st_iic.r_count = st_reg_cus_def_reg3.r_count;
	st_iic.w_count = st_reg_cus_def_reg3.w_count;
}
void rw_cus_def_reg4(void)
{
	st_iic.data_ptr = &(st_reg_cus_def_reg4.data[0]);
	st_iic.r_count = st_reg_cus_def_reg4.r_count;
	st_iic.w_count = st_reg_cus_def_reg4.w_count;
}
void rw_cus_def_reg5(void)
{
	st_iic.data_ptr = &(st_reg_cus_def_reg5.data[0]);
	st_iic.r_count = st_reg_cus_def_reg5.r_count;
	st_iic.w_count = st_reg_cus_def_reg5.w_count;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_data_cmd(uint8_t mode_select)
{
	st_iic.data_ptr = (u8 *)st_interaction[mode_select].cmd_buf;
	st_iic.r_count = CMD_COUNT;
	st_iic.w_count = CMD_COUNT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_data_count(uint8_t mode_select)
{
	st_iic.data_ptr = (u8 *)st_interaction[mode_select].rw_cound_buf;
	st_iic.r_count = RW_COUNT;
	st_iic.w_count = RW_COUNT;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_data_list(uint8_t mode_select)
{
	st_iic.data_ptr = (u8 *)st_interaction[mode_select].rw_data_buf;
	st_iic.r_count = RW_DATA_COUNT;
	st_iic.w_count = RW_DATA_COUNT;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_single_data(uint8_t mode_select, uint8_t data_select)
{
	uint8_t i = 0;

	uint8_t output_data_select = 0;
	uint8_t output_count = 0;
	unsigned short crc_vlaue = 0;

	switch(data_select)
	{
	case DEBUG_MODE_READ_RAWDATA:
		output_data_select = EM_RAW_DATA;
		break;
	case DEBUG_MODE_READ_ADC_VALUE:
		output_data_select = EM_ADC;
		break;
	case DEBUG_MODE_READ_FORCE_VALUE:
		output_data_select = EM_FORCE_VALUE;
		break;
	default:
		break;
	}

	if(st_interaction[mode_select].rw_cound_buf[0] == 0)
	{
		cmd_enter_high_speed_period(); //prevent tangle in low speed prd
		
		for(i = 0; i < st_output[output_data_select].r_count; i++)
		{
			st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)((st_output[output_data_select].data_ptr[i] & 0x00FF));
			st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(st_output[output_data_select].data_ptr[i] >> 8);

			output_count++;
		}

		crc_vlaue = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, output_count << 1);

		st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)((crc_vlaue & 0x00FF));
		st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(crc_vlaue >> 8);

		st_interaction[mode_select].rw_cound_buf[0] = ((output_count << 1) + 2);

		u32 overflow_cnt = 200000;    //about 100ms
		while(st_interaction[mode_select].rw_cound_buf[0] && overflow_cnt--);

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_option_data_head(uint8_t mode_select)
{
	uint8_t i = 0;
	uint8_t output_count = 0;
	unsigned short crc_vlaue;

	if(st_interaction[mode_select].rw_cound_buf[0] == 0)
	{
		for(i = 0; i < EM_OUTPUT_END; i++)
		{
			if((st_output_opt[i].parameter) == 1)
			{
				st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)(st_output[i].sub_head);
				st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(st_output[i].head);

				output_count++;
			}
		}

		crc_vlaue = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, output_count << 1);

		st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)((crc_vlaue & 0x00FF));
		st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(crc_vlaue >> 8);

		st_interaction[mode_select].rw_cound_buf[0] = ((output_count << 1) + 2);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void output_option_data(uint8_t mode_select)
{
	uint8_t i = 0, j = 0;
	uint8_t output_count = 0;
	unsigned short crc_vlaue;

	if(st_interaction[mode_select].rw_cound_buf[0] == 0)
	{
		for(i = 0; i < EM_OUTPUT_END; i++)
		{
			if((st_output_opt[i].parameter) == 1)
			{
				for(j = 0; j < st_output[i].r_count; j++)
				{
					st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)((st_output[i].data_ptr[j] & 0x00FF));
					st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(st_output[i].data_ptr[j] >> 8);

					output_count++;
				}
			}
		}

		crc_vlaue = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, output_count << 1);
		
		st_interaction[mode_select].rw_data_buf[output_count << 1] = (IIcType)((crc_vlaue & 0x00FF));
		st_interaction[mode_select].rw_data_buf[(output_count << 1) + 1] = (IIcType)(crc_vlaue >> 8);

		st_interaction[mode_select].rw_cound_buf[0] = ((output_count << 1) + 2);
    }

}

u8 output_demo_apk_data(void* log)
{
	u8 i = 0;
	s16 *lpdata = (s16*)log;
	u8 len;

//	st_sys_ctr.dis_report_button_cnt = 1000; //1s dis button report
	st_sys_ctr.dis_report_int_cnt = 1000;    //1s dis int report
	st_scan_mode_opt.enter_low_speed_cnt = 0;

	*lpdata++ = st_reg_edge_event.data[1];
	
	for(i = 0; i < CH_COUNT; i++)
	{
		*lpdata++ = st_raw.current_buf[i];
	}

	for(i = 0; i < CH_COUNT; i++)
	{
		*lpdata++ = st_common.hol_balance_buf[i] * 1000;
	}

	for(i = 0; i < CH_COUNT; i++) 
	{
		*lpdata++ = st_fv.force_val[i];
	}

	*lpdata++ = st_ff.all_flag;

//	*lpdata++ = (st_pos_det.cur_pos[0] * 100 ) / st_sensor.model_count[0];

//	*lpdata++ = (s8)st_common.temprature_cur + (s16)((st_common.temprature_init - st_common.temprature_init_bat) << 8);

	for(i = 0; i < PHY_CH_COUNT; i++)
	{
		*lpdata++ = st_common.adc_data[i];
	}
	
//    for(i = 0; i < MAX_TP_COUNT; i++)
//    {
//        *lpdata++ = st_tp_info[i].x_point;
//        *lpdata++ = st_tp_info[i].y_point;
//    }
	
	for(i = 0; i < PHY_CH_COUNT; i++)
	{
		*lpdata++ = st_common.dac_data[i];
	}
	
    *lpdata++ = st_common.temprature_cur * 10; 	//11
	*lpdata++ = st_common.temprature_init * 10;  	//10
    *lpdata++ = st_common.temprature_init_bat * 10;	//12
	*lpdata++ = st_common.temprature_real * 10;	//75
	
	///////////20230213 From hzf/////////////////////
	
	*lpdata++ = st_dac.total_cali_times; //动态校准计数
	
//	for(i = 0; i < PHY_CH_COUNT; i++)//79-71
//    {
//        *lpdata++ = st_common.origin_adc_data[i];	//aw原始adc数据 
//    }

	*lpdata++ = st_sys_ctr.is_factory_pattern;	//工厂模式标志
	*lpdata++ = st_sys_ctr.dis_offset_cancel_cnt;		//禁用动态校准计数低八位
	*lpdata++ = st_sys_ctr.dis_offset_cancel_cnt>>16;	//禁用动态校准计数高八位

	*lpdata++ = st_sys_ctr.period;	//固件运行周期
	
//extern u16 skip_dynamic_calibration;
//	*lpdata++ = skip_dynamic_calibration;	//跳过动态校准的中间变量

	//////////////////////////////////////////////////////////////////
	
	len = lpdata - (s16*)log;

	return len * sizeof(s16);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//debugmode write cali para format: 4byte data * ch_num + 2byte checksum
void dbg_mode_write_cali_para(uint8_t mode_select)
{
	if(st_interaction[mode_select].rw_cound_buf[0] == TOTAL_CALI_PARA_SIZE)
	{
		st_interaction[mode_select].rw_data_buf[0] = hal_write_cali_2_flash_enforce((u8 *)st_interaction[mode_select].rw_data_buf);
		st_interaction[mode_select].rw_cound_buf[0] = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//debugmod 
void dbg_mode_read_all_cali_para(uint8_t mode_select)
{
	u8 ret;
	
	ret = hal_read_all_cali_from_flash((u8 *)st_interaction[mode_select].rw_data_buf);

	if (JOB_OK == ret)
	{
		st_interaction[mode_select].rw_cound_buf[0] = TOTAL_CALI_PARA_SIZE;
	}
	else
	{
		st_interaction[mode_select].rw_data_buf[0] = ret;
		st_interaction[mode_select].rw_cound_buf[0] = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//write model
void dbg_mode_write_single_model(u8 mode_select)
{
	if (st_interaction[mode_select].rw_cound_buf[0] == st_interaction[mode_select].rw_data_buf[0]
		&& st_interaction[mode_select].rw_data_buf[0] > 6
		)
	{
		st_interaction[mode_select].rw_data_buf[0] = hal_write_single_model_2_flash((u8 *)st_interaction[mode_select].rw_data_buf);
		st_interaction[mode_select].rw_cound_buf[0] = 1;

	/////////////////////临时补丁，生效模板！！！！！！！！！！！！！！！！！！！！！
		if (st_sensor.ch_end[0] - 1 == st_interaction[mode_select].rw_data_buf[1])
		{
			hal_flash_para_init();
			ndt_algorithm_init();
		}
	/////////////////////临时补丁，生效模板！！！！！！！！！！！！！！！！！！！！！
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//read model
void dbg_mode_read_single_model(uint8_t mode_select)
{
	u8 index;
	u8 ret;

	if (1 == st_interaction[mode_select].rw_cound_buf[0])
	{
		index = st_interaction[mode_select].rw_data_buf[0];
		
		ret = hal_read_single_model_from_flash(index, (u8 *)st_interaction[mode_select].rw_data_buf);

		if (JOB_OK == ret)
		{
			st_interaction[mode_select].rw_cound_buf[0] = st_interaction[mode_select].rw_data_buf[0];
		}
		else
		{
			st_interaction[mode_select].rw_data_buf[0] = ret;
			st_interaction[mode_select].rw_cound_buf[0] = 1;
		}
	}
}


//write model
void dbg_mode_write_large_model(u8 mode_select)
{
	if (st_interaction[mode_select].rw_cound_buf[0] == st_interaction[mode_select].rw_data_buf[0]
		&& st_interaction[mode_select].rw_data_buf[0] > 6
		)
	{
		st_interaction[mode_select].rw_data_buf[0] = hal_write_large_model_2_flash((u8 *)st_interaction[mode_select].rw_data_buf);
		st_interaction[mode_select].rw_cound_buf[0] = 1;

	/////////////////////临时补丁，生效模板！！！！！！！！！！！！！！！！！！！！！
	#ifdef LAST_LARGE_MODEL_ID
		if (LAST_LARGE_MODEL_ID == st_interaction[mode_select].rw_data_buf[1])
		{
			//here reuse the rw_data_buf to save calitemprature to flash
			s16 csm = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, TOTAL_CALI_PARA_SIZE - 2);
			st_interaction[mode_select].rw_data_buf[TOTAL_CALI_PARA_SIZE - 2] = csm;
			st_interaction[mode_select].rw_data_buf[TOTAL_CALI_PARA_SIZE - 1] = csm >> 8;
			hal_write_cali_2_flash_enforce((u8 *)st_interaction[mode_select].rw_data_buf);

			hal_flash_para_init();
			ndt_algorithm_init();
		}
	#endif
	/////////////////////临时补丁，生效模板！！！！！！！！！！！！！！！！！！！！！
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//read model
void dbg_mode_read_large_model(uint8_t mode_select)
{
	u8 index;
	u8 ret;

	if (1 == st_interaction[mode_select].rw_cound_buf[0])
	{
		index = st_interaction[mode_select].rw_data_buf[0];
		
		ret = hal_read_large_model_from_flash(index, (u8 *)st_interaction[mode_select].rw_data_buf);

		if (JOB_OK == ret)
		{
			st_interaction[mode_select].rw_cound_buf[0] = st_interaction[mode_select].rw_data_buf[0];
		}
		else
		{
			st_interaction[mode_select].rw_data_buf[0] = ret;
			st_interaction[mode_select].rw_cound_buf[0] = 1;
		}

		#if (defined(AW_86802) || defined(CS_f71))
			st_interaction[mode_select].cmd_buf[0] = 0; //clear debugmode
		#endif
	}
}

void dbg_mode_write_ap_test_2_flash(u8 mode_select)
{
	if (st_interaction[mode_select].rw_cound_buf[0] == st_interaction[mode_select].rw_data_buf[0]
		&& st_interaction[mode_select].rw_data_buf[0] > 6
		)
	{
		st_interaction[mode_select].rw_data_buf[0] = hal_write_ap_test_2_flash((u8 *)st_interaction[mode_select].rw_data_buf);
		st_interaction[mode_select].rw_cound_buf[0] = 1;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void dbg_mode_read_ap_test_from_flash(uint8_t mode_select)
{
	u8 ret;

	if (0 == st_interaction[mode_select].rw_cound_buf[0])
	{
		
		ret = hal_read_ap_test_from_flash((u8 *)st_interaction[mode_select].rw_data_buf);

		if (JOB_OK == ret)
		{
			st_interaction[mode_select].rw_cound_buf[0] = st_interaction[mode_select].rw_data_buf[0];
		}
		else
		{
			st_interaction[mode_select].rw_data_buf[0] = ret;
			st_interaction[mode_select].rw_cound_buf[0] = 1;
		}
	}
}



//extern u16 channel_check_val_detail[PHY_CH_COUNT][CHANNEL_CHECK_DETAIL_CNT];

void dbg_mode_read_sensor_conn_info(uint8_t mode_select)
{
	u8 i = 0, j = 0;
	s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
	u8 len;

	//涓诲姩鎶ラ敭
	//st_reg_edge_event.data[1] = 0x3;
	
	if(0 == st_interaction[mode_select].rw_cound_buf[0])
	{
//		hal_channel_check(PHY_CH_COUNT);
		cmd_enter_high_speed_period();

#ifdef F61_DAC_SCAN_EN
extern int dac_scan(int phy_ch, uint32_t phy_vs, int16_t *res);
		int16_t adc_delta;
		int16_t dac_test_res;
		dac_test_res = dac_scan(st_hal_platform.ch_buf[PHY_CH_COUNT - 1], st_hal_platform.power_switch[PHY_CH_COUNT - 1], &adc_delta);
#endif

#ifdef DBG_MODE_SELF_TEST_AUTO_TOGGLE_IO_EN
		cus_cmd_detect(CMD_IO_AUTO_TEST);
#endif

#ifdef F61_DAC_SCAN_EN
		if (dac_test_res)
		{
			*lpdata++ = 0xff;
		}
		else
#endif
		{
			*lpdata++ = ~st_hal_platform.ch_flag;
		}

		for (i = 0; i < PHY_CH_COUNT; i++)
		{
			for (j = 0; j < CHANNEL_CHECK_DETAIL_CNT; j++)
			{
//				*lpdata++ = channel_check_val_detail[i][j];
			}
		}

		*lpdata++ = st_common.temprature_init | ((s16)st_common.temprature_init_bat << 8);
		
#ifdef F61_DAC_SCAN_EN
		*lpdata++ = adc_delta;
#endif
		
		len = lpdata - (s16 *)&st_interaction[mode_select].rw_data_buf[0];

		*lpdata = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, len << 1);

		st_interaction[mode_select].rw_cound_buf[0] = ((len << 1) + 2);
	}
}

