/*************************************************
* NDT REG TABLE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __REG_TABLE_H
#define __REG_TABLE_H

#include <stdbool.h>
#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Global Function */
#define REG_IIC_TEST		0x00
#define REG_HANDSHAKE		0x01
#define REG_CMD				0x02
#define REG_HEART_BEAT		0X03

#define REG_COUNT_INFO		0x06 //0x1F //0x06

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For System Info [] */
#define REG_MCU_ID		0x0B
#define REG_RST_SRC_ID  0x0C
#define REG_PROJECT_ID	0x0F
#define REG_FW_VER		0x10

#define CUSTOM_SCREEN_INFO  0x09

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For AFE Info [] */
#define REG_AFE_GAIN		0x16
#define REG_DAC_VALUE		0x17

#define REG_ADC_2_UVOLT_COEF	0x1B
#define REG_DAC_2_UVOLT_COEF	0x1C

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Sensor Info [] */
#define REG_GROUP_MAP_INFO	0x1F //0x1E //0x1F

#define REG_NOISE_THR 0X21
#define REG_CHIP_NOISE_THR 0X22
#define REG_OFFSET_THR 0X23

#define REG_BALANCE_INFO	0x24
#define REG_TEMP_INFO		0x25

#define REG_CHANNEL_GAP		0x28

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Platform Info [] */
#define REG_CH_CONN_STATUS	0x29

#define REG_GPIO_STATUS		0x2E

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For DEBUG Info [] */

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Edge Func Control [] */
#define REG_TIME_OUT		0x51

#define REG_BATTERY_TEMP  		0x54

#define REG_AP_STATUS  		0x55
#define REG_SCAN_MODE_SWITCH	0x56
#define REG_FUN_SWITCH		0x57

#define REG_BUTTON_FORCE_THR	0x5B
#define REG_BUTTON_POS		0x5C

#define REG_SLIDE_FORCE_THR	0x60

#define REG_CUS_DEFINE1     0xB5
#define REG_CUS_DEFINE2     0xB6
#define REG_CUS_DEFINE3     0xB7
#define REG_CUS_DEFINE4     0xB8
#define REG_CUS_DEFINE5     0xB9


///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Event Status */
#define REG_EDGE_EVENT		0xAB

///////////////////////////////////////////////////////////////////////////////////////////////////

#define MODE_A_CMD              0xEC
#define MODE_A_COUNT            0xED
#define MODE_A_LIST             0xEE

#define MODE_B_CMD              0xF1
#define MODE_B_COUNT            0xF2
#define MODE_B_LIST             0xF3

#define MODE_C_CMD              0xF6
#define MODE_C_COUNT            0xF7
#define MODE_C_LIST             0xF8

#define MODE_D_CMD              0xFB
#define MODE_D_COUNT            0xFC
#define MODE_D_LIST             0xFD

#define DEBUG_MODE_READ_RAWDATA    			(0X10)
#define DEBUG_MODE_READ_NOISE_VALUE    		(0X11)
#define DEBUG_MODE_READ_ADC_VALUE    		(0X14)
#define DEBUG_MODE_READ_FORCE_VALUE    		(0X20)
#define DEBUG_MODE_READ_SINGLE_FORCE    	(0X21)

#define DEBUG_MODE_WRITE_CAL_DATA	        (0x30)
#define DEBUG_MODE_READ_CAL_DATA	        (0x31)
#define DEBUG_MODE_CALC_CALI_PARA           (0x32)

#define DEBUG_MODE_WRITE_CH_MODEL			(0x38)
#define DEBUG_MODE_READ_CH_MODEL			(0x39)
#define DEBUG_MODE_READ_OPTION_DATA			(0x40)
#define DEBUG_MODE_READ_OPTION_HEAD			(0x41)

#define DEBUG_MODE_WRITE_LARGE_MODEL		(0x42)
#define DEBUG_MODE_READ_LARGE_MODEL			(0x43)
#define DEBUG_MODE_WRITE_AP_TEST_DATA		(0x44)
#define DEBUG_MODE_READ_AP_TEST_DATA		(0x45)

#define DEBUG_MODE_READ_SENSOR_CONN_INFO 	(0xA4)
#define DEBUG_MODE_READ_OFFSET_RESULT       (0XA6)
#define DEBUG_MODE_READ_CHIP_NOISE_RESULT   (0XA7)
#define DEBUG_MODE_READ_NOISE_RESULT        (0XA8)

#define DEBUG_MODE_READ_DEMO_UNSYNC			(0XF2)
#define DEBUG_MODE_READ_DEMO_SYNC			(0XF3)
#define DEBUG_MODE_READ_DEMO_SYNC_IOMASK	(0XF4)
#define DEBUG_MODE_READ_USE_DBG_DATA        (0XF5)

///////////////////////////////////////////////////////////////////////////////////////////////////
extern void rw_null_reg(void);
extern void rw_iic_test(void);
extern void rw_handshake(void);
extern void rw_cmd(void);
extern void rw_heart_beat(void);
extern void read_count_info(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void read_mcu_id(void);
extern void read_rst_src_id(void);
extern void read_project_id(void);
extern void read_fw_ver(void);
extern void read_screen_info(void);
///////////////////////////////////////////////////////////////////////////////////////////////////

extern void read_afe_gain(void);
extern void read_dac_value(void);
extern void read_adc_2_uvolt_coef(void);
extern void read_dac_2_uvolt_coef(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void read_group_map_info(void);
extern void read_noise_thr(void);
extern void read_chip_noise_thr(void);
extern void read_offset_thr(void);
extern void read_balance_info(void);
extern void read_temp_info(void);
extern void read_channel_gap(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void read_ch_conn_status(void);
extern void read_gpio_status(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void rw_time_out(void);
extern void rw_battery_temp(void);
extern void rw_ap_status(void);
extern void rw_scan_mode_switch(void);
extern void rw_fun_switch(void);
extern void rw_button_force_thr(void);
extern void rw_button_pos(void);
extern void rw_slide_force_thr(void);
extern void rw_cus_def_reg1(void);
extern void rw_cus_def_reg2(void);
extern void rw_cus_def_reg3(void);
extern void rw_cus_def_reg4(void);
extern void rw_cus_def_reg5(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void rw_edge_event(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void read_event_val(void);
extern void read_key_event(void);
extern void read_ch_info(uint8_t mode_select);

extern void output_data_cmd(uint8_t mode_select);
extern void output_data_count(uint8_t mode_select);
extern void output_data_list(uint8_t mode_select);

extern void output_single_data(uint8_t mode_select, uint8_t data_select);

extern void output_option_data(uint8_t mode_select);
extern void output_option_data_head(uint8_t mode_select);
unsigned char output_demo_apk_data(void* log);
//extern void output_user_debug_data(uint8_t mode_select);
extern unsigned char output_user_debug_data(void* log);

extern void dbg_mode_write_cali_para(uint8_t mode_select);
extern void dbg_mode_read_all_cali_para(uint8_t mode_select);
extern void dbg_mode_write_single_model(uint8_t mode_select);
extern void dbg_mode_read_single_model(uint8_t mode_select);
extern void dbg_mode_read_sensor_conn_info(uint8_t mode_select);
extern void dbg_mode_write_large_model(uint8_t mode_select);
extern void dbg_mode_read_large_model(uint8_t mode_select);
extern void dbg_mode_write_ap_test_2_flash(uint8_t mode_select);
extern void dbg_mode_read_ap_test_from_flash(uint8_t mode_select);
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif  /* __REG_TABLE_H */
