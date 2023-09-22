///******************************************************************************
//* NDT USER INTERFACE
//* Copyright(c) 2018, NewDegreeTechnology, Inc.
//*******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "user_interface.h"
#include "reg_table.h"
#include "algorithm_init.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

volatile ST_IIC st_iic = {0};

volatile ST_INTERACTION st_interaction[MODE_COUNT] = {0};
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void register_select(IIcType iic_data)
//{
//	
//#ifdef THIRD_PARTY_PROTOCOL
//	if (third_party_reg_sel(iic_data))
//	{
//		return;
//	}
//#endif
//
//	switch(iic_data)
//	{
//	/////////////////////////////////////////////////////////////
//
//	case REG_IIC_TEST:
//		rw_iic_test();
//		break;
//
//	case REG_HANDSHAKE:
//		rw_handshake();
//		break;
//
//	case REG_CMD:
//		rw_cmd();
//		break;
//
//	case REG_HEART_BEAT:
//		rw_heart_beat();
//		break;
//
//	case REG_COUNT_INFO:
//		read_count_info();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_MCU_ID:
//		read_mcu_id();
//		break;
//	
//	case REG_RST_SRC_ID:
//		read_rst_src_id();
//		break;
//
//	case REG_PROJECT_ID:
//		read_project_id();
//		break;
//
//	case REG_FW_VER:
//		read_fw_ver();
//		break;
//    
//    case CUSTOM_SCREEN_INFO:
//        read_screen_info();
//        break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_AFE_GAIN:
//		read_afe_gain();
//		break;
//
//	case REG_DAC_VALUE:
//		read_dac_value();
//		break;
//
//	case REG_ADC_2_UVOLT_COEF:
//		read_adc_2_uvolt_coef();
//		break;
//
//	case REG_DAC_2_UVOLT_COEF:
//		read_dac_2_uvolt_coef();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_GROUP_MAP_INFO:
//		read_group_map_info();
//		break;
//
//	case REG_NOISE_THR:
//		read_noise_thr();
//		break;
//
//	case REG_CHIP_NOISE_THR:
//		read_chip_noise_thr();
//		break;
//
//	case REG_OFFSET_THR:
//		read_offset_thr();
//		break;
//
//	case REG_BALANCE_INFO:
//		read_balance_info();
//		break;
//
//	case REG_TEMP_INFO:
//		read_temp_info();
//		break;
//
//	case REG_CHANNEL_GAP:
//		read_channel_gap();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_CH_CONN_STATUS:
//		read_ch_conn_status();
//		break;
//
//	case REG_GPIO_STATUS:
//		read_gpio_status();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_TIME_OUT:
//		rw_time_out();
//		break;
//
//	case REG_BATTERY_TEMP:
//		rw_battery_temp();
//		break;
//	
//	case REG_AP_STATUS:
//		rw_ap_status();
//		break;
//
//	case REG_SCAN_MODE_SWITCH:
//		rw_scan_mode_switch();
//		break;
//
//	case REG_FUN_SWITCH:
//		rw_fun_switch();
//		break;
//
//	case REG_BUTTON_FORCE_THR:
//		rw_button_force_thr();
//		break;
//
//	case REG_BUTTON_POS:
//		rw_button_pos();
//		break;
//
//	case REG_SLIDE_FORCE_THR:
//		rw_slide_force_thr();
//		break;
//
//#ifdef AW_86802
//	case 0x85:
//	hal_uboot_read_fw_vresion();
//	break;
//#endif
//
//	case REG_CUS_DEFINE1:
//		rw_cus_def_reg1();
//		break;
//	
//	case REG_CUS_DEFINE2:
//		rw_cus_def_reg2();
//		break;
//	
//	case REG_CUS_DEFINE3:
//		rw_cus_def_reg3();
//		break;
//	case REG_CUS_DEFINE4:
//		rw_cus_def_reg4();
//		break;
//	case REG_CUS_DEFINE5:
//		rw_cus_def_reg5();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	case REG_EDGE_EVENT:
//		rw_edge_event();
//		break;
//
//	/////////////////////////////////////////////////////////////
//
//	/////////////////////////////////////////////////////////////
//
//	case MODE_A_CMD:
//		output_data_cmd(0);
//		break;
//
//	case MODE_A_COUNT:
//		output_data_count(0);
//		break;
//
//	case MODE_A_LIST:
//		output_data_list(0);
//		break;
//
//	case MODE_B_CMD:
//		output_data_cmd(1);
//		break;
//
//	case MODE_B_COUNT:
//		output_data_count(1);
//		break;
//
//	case MODE_B_LIST:
//		output_data_list(1);
//		break;
//
//	case MODE_C_CMD:
//		output_data_cmd(2);
//		break;
//
//	case MODE_C_COUNT:
//		output_data_count(2);
//		break;
//
//	case MODE_C_LIST:
//		output_data_list(2);
//		break;
//
//	case MODE_D_CMD:
//        output_data_cmd(3);
//        break;
//
//    case MODE_D_COUNT:
//        output_data_count(3);
//        break;
//
//    case MODE_D_LIST:
//        output_data_list(3);
//        break;
//
//	default:
//		rw_null_reg();
//		break;
//	}
//}
//
//extern void update_reg_data_to_chip_temperature(void);
//////////////////////////////////////////////////////////////////////////////////////////////////////
//void sys_cmd_detect(void)
//{
//	bool run_finish = true;
//	static u16 cmd_cn_state_cnt = 0; //chip noise err cnt
//
//	if( 0 != st_reg_cmd.data[0]
//	    && 0 == ((st_reg_cmd.data[0] + st_reg_cmd.data[1]) & 0xff)
//	)
//	{
//		switch(st_reg_cmd.data[0])
//		{
//			case CMD_SW_RESET:
//#ifdef AW_8686X                
////                System_Reset(MCU_Reset);
//#endif //AW_8686X            
//				break;
//
//			case CMD_RESET_FORCE_FLAG:
//	            break;
//
//	        case CMD_RESET_BASE_LINE:
//	            break;
//				
//			case CMD_OFFSET_CANCEL:
//				cmd_offset_cancel(1);
//				cmd_enter_high_speed_period();
//                update_reg_data_to_chip_temperature();
//				//clear algorithm status
//				for (int i = 0; i < GR_COUNT; i++)
//				{
//					st_base.ok_flag[i] = 0;
//					st_ff.sub_flag[i] = 0;
//				}
//			break;
//
//			case CMD_OFFSET_CANCEL_EXIT:
//				cmd_offset_cancel(0);
//			break;
//
//			case CMD_IO_AUTO_TEST:
//			case CMD_GPIO_STATUS:
//			case CMD_CUS_DEFINE_0:
//			case CMD_CUS_DEFINE_1:
//			case CMD_CUS_DEFINE_2:
//			case CMD_CUS_DEFINE_3:
//			case CMD_CUS_DEFINE_4:
//			case CMD_CUS_DEFINE_5:
//				cus_cmd_detect(st_reg_cmd.data[0]);
//				break;
//
//			case CMD_ENTER_CHIP_NOISE_MODE:
//                update_reg_data_to_chip_temperature();
////				hal_afe_noise_test(true);//内短
//				ndt_raw_data_init();
//
//				//to save time, immediately jump to correct state.
//				for (int i = 0; i < PHY_CH_COUNT; i++)
//				{
////					hal_dac_set(i, 0);
//				}
//				
//				st_control_flag.chip_short_state = true;
//				st_sys_ctr.dis_offset_cancel_cnt = 0;
//				cmd_enter_high_speed_period();
//			break;
//
//			case CMD_EXIT_CHIP_NOISE_MODE:
////				hal_afe_noise_test(false);//内短
//				ndt_raw_data_init();
//				st_control_flag.chip_short_state = false;
//				st_dac.crazy_cancel_cnt = 0;
//			break;
//			
//			default:
//				run_finish = false;
//				break;
//		}
//
//		if(run_finish)
//        {
//            st_reg_cmd.data[0] = 0x00;
//            st_reg_cmd.data[1] = 0x00;
//        }
//	}
//
//	if (st_control_flag.chip_short_state)
//	{
//		cmd_cn_state_cnt += st_sys_ctr.period;
//		if (cmd_cn_state_cnt > 7000)  // force exit chip noise mode
//		{
////			hal_afe_noise_test(false);	//内短
//			ndt_raw_data_init();
//			st_control_flag.chip_short_state = false;
//		}
//	}
//	else
//	{
//		cmd_cn_state_cnt = 0;
//	}
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void sys_handshake_detect(void)
//{
//	u8 i = 0;
//	u16 temp_parameter;
//	bool update_finish = true;
//
//	if(0 != st_reg_handshake.data[0]
//	&& 0 == ((st_reg_handshake.data[0] + st_reg_handshake.data[1]) & 0xff)
//	)
//	{
//		switch (st_reg_handshake.data[0])
//		{
//			case REG_TIME_OUT:
//				for (i = 0; i < GR_COUNT; i++)
//				{
//					st_time_out.thr[i] = st_reg_time_out.data[0] + (st_reg_time_out.data[1] << 8);
//#ifdef TIME_OUT_UPPER_LIMIT_THR
//					st_time_out.thr[i] = MIN(st_time_out.thr[i], TIME_OUT_UPPER_LIMIT_THR);
//					st_time_out.thr[i] = MAX(st_time_out.thr[i], TIME_OUT_LOWER_LIMIT_THR);
//#endif
//				}
//				break;
//
//			case REG_SCAN_MODE_SWITCH:
//					st_scan_mode_opt.cur_value = (EM_SCAN_MODE_OPT)st_reg_scan_mode_switch.data[0];
//				break;
//			
//			case REG_BUTTON_FORCE_THR:
//				for(i = 0; i < BU_COUNT; i++)
//				{
//					temp_parameter = (st_reg_button_force_thr.data[(i * 4) + 1] << 8) | (st_reg_button_force_thr.data[(i * 4)]);
//					st_algo_para.button_tri_thr[i] = temp_parameter;
//
//					temp_parameter = (st_reg_button_force_thr.data[(i * 4) + 3] << 8) | (st_reg_button_force_thr.data[(i * 4) + 2]);
//					st_algo_para.button_rel_thr[i] = MIN(temp_parameter, st_algo_para.button_tri_thr[i]);
//				}
//
//#ifdef EN_WRITE_BUTTON_THR_2_FLASH
//				write_button_thr_2_flash();
//#endif
//
//	            break;
//
//			case REG_BUTTON_POS:
//				for(i = 0; i < BU_COUNT; i++)
//				{
//					temp_parameter = (st_reg_button_pos.data[(i * 4) + 1] << 8) | (st_reg_button_pos.data[(i * 4)]);
//					st_algo_para.button_pos_start[i] = temp_parameter;
//					st_algo_para.button_pos_start_wide[i] = temp_parameter - 1;
//
//					temp_parameter = (st_reg_button_pos.data[(i * 4) + 3] << 8) | (st_reg_button_pos.data[(i * 4) + 2]);
//					st_algo_para.button_pos_end[i] = temp_parameter;
//					st_algo_para.button_pos_end_wide[i] = temp_parameter + 1;
//	            }
//				break;
//
//			case REG_SLIDE_FORCE_THR:
//				for(i = 0; i < SA_COUNT; i++)
//				{
//					temp_parameter = (st_reg_slide_force_thr.data[(i * 4) + 1] << 8) | (st_reg_slide_force_thr.data[(i * 4)]);
//					st_algo_para.slide_tri_thr[i] = temp_parameter;
//
//					temp_parameter = (st_reg_slide_force_thr.data[(i * 4) + 3] << 8) | (st_reg_slide_force_thr.data[(i * 4) + 2]);
//					st_algo_para.slide_rel_thr[i] = temp_parameter;
//				}
//				break;
//
//			case REG_AP_STATUS:
//				st_hal_platform.ap_status = st_reg_ap_status.data[0];
//			case REG_CUS_DEFINE1:
//			case REG_CUS_DEFINE2:
//			case REG_CUS_DEFINE3:
//			case REG_CUS_DEFINE4:
//			case REG_CUS_DEFINE5:
//				cus_handshake_detect(st_reg_handshake.data[0]);
//				break;
//
//			default:
//				update_finish = false;
//				break;
//		}
//
//		if(update_finish)
//		{
//			st_reg_handshake.data[0] = 0x00;
//			st_reg_handshake.data[1] = 0x00;
//		}
//	}
//}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void dynamic_register_interaction(void)
//{
//	u8 i = 0;
//
//	for(i = 0; i < MODE_COUNT; i++)
//	{
//		switch(st_interaction[i].cmd_buf[0])
//		{
//			case DEBUG_MODE_WRITE_CAL_DATA:
//				dbg_mode_write_cali_para(i);
//				break;
//
//			case DEBUG_MODE_READ_CAL_DATA:
//				dbg_mode_read_all_cali_para(i);
//				break;
//
//			case DEBUG_MODE_WRITE_CH_MODEL:
//				dbg_mode_write_single_model(i);
//				break;
//
//			case DEBUG_MODE_READ_CH_MODEL:
//				dbg_mode_read_single_model(i);
//				break;
//
//			case DEBUG_MODE_WRITE_LARGE_MODEL:
//				dbg_mode_write_large_model(i);
//				break;
//
//			case DEBUG_MODE_READ_LARGE_MODEL:
//				dbg_mode_read_large_model(i);
//				break;
//
//			case DEBUG_MODE_READ_AP_TEST_DATA:
//				dbg_mode_read_ap_test_from_flash(i);
//				break;
//
//			case DEBUG_MODE_WRITE_AP_TEST_DATA:
//				dbg_mode_write_ap_test_2_flash(i);
//				break;
//			
//			case DEBUG_MODE_READ_SENSOR_CONN_INFO:
//				dbg_mode_read_sensor_conn_info(i);
//				break;
//			
//#ifdef EN_NOISE_CALC_BY_FW
//			case DEBUG_MODE_READ_NOISE_VALUE:
//				dbg_mode_read_noise_val(i);
//				break;
//#endif
//
//#ifdef EN_RESULT_CALC_BY_FW
//			case DEBUG_MODE_READ_OFFSET_RESULT:
//				dbg_mode_read_offset_res(i);
//				break;
//
//			case DEBUG_MODE_READ_CHIP_NOISE_RESULT:
//				dbg_mode_read_chip_noise_res(i);
//				break;
//
//			case DEBUG_MODE_READ_NOISE_RESULT:
//				dbg_mode_read_noise_res(i);
//				break;
//#endif
//
//#ifdef EN_CALI_PARA_CALC_BY_FW
//			case DEBUG_MODE_CALC_CALI_PARA:
//				dbg_mode_calc_cali_para(i);
//				break;
//#endif
//
//			case DEBUG_MODE_READ_RAWDATA: //rawdata
//			case DEBUG_MODE_READ_ADC_VALUE: //adc data
//			case DEBUG_MODE_READ_FORCE_VALUE: //force value
//				output_single_data(i, st_interaction[i].cmd_buf[0]);
//				break;
//
//#ifdef EN_OUTP_SINGLE_FORCE
//			case DEBUG_MODE_READ_SINGLE_FORCE:
//				output_single_force(i);
//				break;
//#endif  //EN_OUTP_SINGLE_FORCE
//
//			case DEBUG_MODE_READ_OPTION_DATA:
//				output_option_data(i);
//				break;
//
//			case DEBUG_MODE_READ_OPTION_HEAD:
//				output_option_data_head(i);
//				break;
//
//			case DEBUG_MODE_READ_DEMO_UNSYNC:  //kernel log without sync
//			case DEBUG_MODE_READ_DEMO_SYNC:  //kernel log with sync
//			case DEBUG_MODE_READ_DEMO_SYNC_IOMASK:  //demo apk log with sync && io mask
//				output_demo_apk_data(i, st_interaction[i].cmd_buf[0]);
//				break;
//			
//			case DEBUG_MODE_READ_USE_DBG_DATA:
//				output_user_debug_data(i);
//				break;
//
//			default:
//				break;
//		}
//	}
//}
//
//
//void interaction_interface(void)
//{
//	sys_cmd_detect();
//
//	sys_handshake_detect();
//
//	dynamic_register_interaction();
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
