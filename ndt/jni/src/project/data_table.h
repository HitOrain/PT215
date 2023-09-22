/*************************************************
* NDT DATA TABLE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __DATA_TABLE_H
#define __DATA_TABLE_H

#include "compile_option.h"
#include "hal_ndt_common.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
#define SPILT_TWO_BYTE(x)       	(x & 0x00FF), (x >> 8) //(x >> 8), (x & 0x00FF)

#define GPIO_NO_SETTING			3

//#define GPIO_INPUT			0
//#define GPIO_OUTPUT			1
//#define GPIO_OPENDRAIN			2

#define GPIO_ENABLE_PULL_UP		0
#define GPIO_DISABLE_PULL_UP		1

#define GPIO_LOW			0
#define GPIO_HIGH			1

#define GPIO_STATUS_INIT(x, y, z)	((x << 6) | (y << 4) | (z << 2) | (z))
#define GPIO_STATUS_UPDATE(x, y)	((x & 0xFC) | (y))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For I2C Test */
#define IIC_TEST_LEN		10

typedef struct{
	u8 data[IIC_TEST_LEN];
	u8 r_count;
	u8 w_count;

} ST_IIC_TEST;

extern ST_IIC_TEST st_reg_iic_test;

///////////////////////////////////////////////////////////////////////////////

/* Define For HANDSHAKE */
#define HANDSHAKE_LEN		2

typedef struct{
	u8 data[HANDSHAKE_LEN];
	u8 r_count;
	u8 w_count;

} ST_HANDSHAKE;

extern ST_HANDSHAKE st_reg_handshake;

///////////////////////////////////////////////////////////////////////////////HEART_BEAT_LEN

/* Define For ST_HEART_BEAT */
#define HEART_BEAT_LEN		4

typedef struct{
	u8 data[HEART_BEAT_LEN];
	u8 r_count;
	u8 w_count;

} ST_HEART_BEAT;

extern ST_HEART_BEAT st_reg_heart;

///////////////////////////////////////////////////////////////////////////////

/* Define For CMD */
#define CMD_LEN			2

typedef struct{
	u8 data[CMD_LEN];
	u8 r_count;
	u8 w_count;

} ST_CMD;

extern ST_CMD st_reg_cmd;

///////////////////////////////////////////////////////////////////////////////

/* Define For COUNT INFO. */
#define COUNT_INFO_LEN		8

typedef struct{
	u8 data[COUNT_INFO_LEN];
	u8 r_count;
	u8 w_count;

} ST_COUNT_INFO;

extern ST_COUNT_INFO st_reg_count_info;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For MCU ID */
typedef struct{
	u8 data[MD_COUNT];
	u8 r_count;
	u8 w_count;

} ST_MCU_ID;

extern ST_MCU_ID st_reg_mcu_id;

///////////////////////////////////////////////////////////////////////////////

/* Define For rst src ID */
typedef struct{
	u8 data[RST_SRC_COUNT];
	u8 r_count;
	u8 w_count;

} ST_RST_SRC;

extern ST_RST_SRC st_reg_rst_src;

///////////////////////////////////////////////////////////////////////////////


/* Define For Project ID */
#define PROJECT_ID_LEN		2

typedef struct{
	u8 data[PROJECT_ID_LEN];
	u8 r_count;
	u8 w_count;

} ST_PROJECT_ID;

extern ST_PROJECT_ID st_reg_project_id;

///////////////////////////////////////////////////////////////////////////////

/* Define For FW VERSION */
#define FW_VER_LEN		6

typedef struct{
	u8 data[FW_VER_LEN];
	u8 r_count;
	u8 w_count;

} ST_FW_VER;

extern ST_FW_VER st_reg_fw_ver;
///////////////////////////////////////////////////////////////////////////////

#if defined(SCREEN_INFO_SUPPORT)
typedef struct SCREEN_INFO
{
    uint16_t x_pixel_num;
    uint16_t y_pixel_num;

    float x_size_mm;
    float y_size_mm;

    uint8_t sample_matrix_rows_num;
    uint8_t sample_matrix_columns_num;
    float matrix_x0;
    float matrix_y0;
}SCREEN_INFO_S;
extern SCREEN_INFO_S m_screen_info;
#endif //SCREEN_INFO_SUPPORT

///////////////////////////////////////////////////////////////////////////////
/* Define For FW VERSION */
#define SCR_INFO_LEN		(16+2)      //2 byte checksum

typedef struct{
	u8 data[SCR_INFO_LEN];
	u8 r_count;
	u8 w_count;

} CUS_SCREEN_INFO;

extern CUS_SCREEN_INFO reg_screen_info;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For AFE GAIN */

typedef struct{
	u8 data[GA_COUNT];
	u8 r_count;
	u8 w_count;

} ST_AFE_GAIN;

extern ST_AFE_GAIN st_reg_afe_gain;

///////////////////////////////////////////////////////////////////////////////

/* Define For DAC Value */
#define DAC_VALUE_LEN		(PHY_CH_COUNT * 2)

typedef struct{
	u8 data[DAC_VALUE_LEN];
	u8 r_count;
	u8 w_count;

} ST_DAC_VALUE;

extern ST_DAC_VALUE st_reg_dac_value;

///////////////////////////////////////////////////////////////////////////////

/* Define For ADC 2 UVOLT COEF */
#define ADC_2_UVOLT_COEF_LEN	3

typedef struct{
	u8 data[ADC_2_UVOLT_COEF_LEN];
	u8 r_count;
	u8 w_count;

} ST_ADC_2_UVOLT_COEF;

extern ST_ADC_2_UVOLT_COEF st_reg_adc_2_uvolt_coef;

///////////////////////////////////////////////////////////////////////////////

/* Define For DAC 2 UVOLT COEF */
#define DAC_2_UVOLT_COEF_LEN	2

typedef struct{
	u8 data[DAC_2_UVOLT_COEF_LEN];
	u8 r_count;
	u8 w_count;

} ST_DAC_2_UVOLT_COEF;

extern ST_DAC_2_UVOLT_COEF st_reg_dac_2_uvolt_coef;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define for Group Map INFO. */
#define GROUP_MAP_INFO_LEN	(GR_COUNT * 2)

typedef struct{ 
	u8 data[GROUP_MAP_INFO_LEN];
	u8 r_count;
	u8 w_count;

} ST_GROUP_MAP_INFO;

extern ST_GROUP_MAP_INFO st_reg_group_map_info;

#define NOISE_THR_LEN	(PHY_CH_COUNT * 4 + 6)

typedef struct{ 
	u8 data[NOISE_THR_LEN];
	u8 r_count;
	u8 w_count;

} ST_NOIST_THR;

extern ST_NOIST_THR st_reg_noise_thr;
extern ST_NOIST_THR st_reg_chip_noise_thr;


#define OFFSET_THR_LEN	(PHY_CH_COUNT * 2)

typedef struct{ 
	u8 data[OFFSET_THR_LEN];
	u8 r_count;
	u8 w_count;

} ST_OFFSET_THR;

extern ST_OFFSET_THR st_reg_offset_thr;

///////////////////////////////////////////////////////////////////////////////

/* Define For Balance INFO. */
#define BALANCE_INFO_LEN	(PHY_CH_COUNT * 2)

typedef struct{
	u8 data[BALANCE_INFO_LEN];
	u8 r_count;
	u8 w_count;

} ST_BALANCE_INFO;

extern ST_BALANCE_INFO st_reg_balance_info;

///////////////////////////////////////////////////////////////////////////////

/* Define For TEMP INFO. */
#define TEMP_INFO_LEN		(4 + (PHY_CH_COUNT * 2))

typedef struct{
	u8 data[TEMP_INFO_LEN];
	u8 r_count;
	u8 w_count;

} ST_TEMP_INFO;

extern ST_TEMP_INFO st_reg_temp_info;

///////////////////////////////////////////////////////////////////////////////

/* Define For Channel GAP */
#if (PHY_CH_COUNT < 2)
#define CHANNEL_GAP_LEN     2
#else
#define CHANNEL_GAP_LEN		((PHY_CH_COUNT - 1) * 2)
#endif

typedef struct{
	u8 data[CHANNEL_GAP_LEN];
	u8 r_count;
	u8 w_count;

} ST_CHANNEL_GAP;

extern ST_CHANNEL_GAP st_reg_channel_gap;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Channel Connect Status */
#define CH_CONN_STATUS_LEN	2

typedef struct{
	u8 data[CH_CONN_STATUS_LEN];
	u8 r_count;
	u8 w_count;

} ST_CH_CONN_STATUS;

extern ST_CH_CONN_STATUS st_reg_ch_conn_status;

///////////////////////////////////////////////////////////////////////////////

/* Define For GPIO Status */
//#define GPIO_STATUS_LEN		2

typedef struct{
	u8 data[GP_COUNT * 2];
	u8 r_count;
	u8 w_count;

} ST_GPIO_STATUS;

extern ST_GPIO_STATUS st_reg_gpio_status;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Time Out */
#define REG_TIME_OUT_LEN		2

typedef struct{
	u8 data[REG_TIME_OUT_LEN];
	u8 r_count;
	u8 w_count;

} ST_REG_TIME_OUT;

extern ST_REG_TIME_OUT st_reg_time_out;


///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For phone battery temperature */
#define REG_BATTERY_TEMP_LEN		2
#ifndef DEFALUT_INIT_TEMP_VAL
#define DEFALUT_INIT_TEMP_VAL 30
#endif

typedef struct{
	u8 data[REG_BATTERY_TEMP_LEN];
	u8 r_count;
	u8 w_count;

} ST_REG_BATTERY_TEMPERATURE;

extern ST_REG_BATTERY_TEMPERATURE st_reg_battery_temp;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Ap status */
#define REG_AP_STATUS_LEN		1

typedef struct{
	u8 data[REG_AP_STATUS_LEN];
	u8 r_count;
	u8 w_count;

} ST_REG_AP_STATUS;

extern ST_REG_AP_STATUS st_reg_ap_status;


///////////////////////////////////////////////////////////////////////////////

/* Define For Scan Mode Switch */
#define SCAN_MODE_SWITCH_LEN		1

typedef struct{
	u8 data[SCAN_MODE_SWITCH_LEN];
	u8 r_count;
	u8 w_count;

} ST_SCAN_MODE_SWITCH;

extern ST_SCAN_MODE_SWITCH st_reg_scan_mode_switch;

///////////////////////////////////////////////////////////////////////////////

/* Define For Fun Switch */
#define FUN_SWITCH_LEN			1

typedef struct{
	u8 data[FUN_SWITCH_LEN];
	u8 r_count;
	u8 w_count;

} ST_FUN_SWITCH;

extern ST_FUN_SWITCH st_reg_fun_switch;

///////////////////////////////////////////////////////////////////////////////

/* Define For Button Force THR */
#define BUTTON_FORCE_THR_LEN		(BU_COUNT * 4)

typedef struct{
	u8 data[BUTTON_FORCE_THR_LEN];
	u8 r_count;
	u8 w_count;

} ST_BUTTON_FORCE_THR;

extern ST_BUTTON_FORCE_THR st_reg_button_force_thr;

///////////////////////////////////////////////////////////////////////////////

/* Define For Button Position */
#define BUTTON_POS_LEN			(BU_COUNT * 4)

typedef struct{
	u8 data[BUTTON_POS_LEN];
	u8 r_count;
	u8 w_count;

} ST_BUTTON_POS;

extern ST_BUTTON_POS st_reg_button_pos;

///////////////////////////////////////////////////////////////////////////////

/* Define For Slide Force THR */
#define SLIDE_FORCE_THR_LEN		(SA_COUNT * 4)

typedef struct{
	u8 data[SLIDE_FORCE_THR_LEN];
	u8 r_count;
	u8 w_count;

} ST_SLIDE_FORCE_THR;

extern ST_SLIDE_FORCE_THR st_reg_slide_force_thr;

#ifndef CUS_DEF_REG1_LEN
#define CUS_DEF_REG1_LEN 2
#endif

typedef struct{
	u8 data[CUS_DEF_REG1_LEN];
	u8 r_count;
	u8 w_count;

} ST_CUS_DEFINE_REG1;

extern ST_CUS_DEFINE_REG1 st_reg_cus_def_reg1;

#ifndef CUS_DEF_REG2_LEN
#define CUS_DEF_REG2_LEN 2
#endif

typedef struct{
	u8 data[CUS_DEF_REG2_LEN];
	u8 r_count;
	u8 w_count;

} ST_CUS_DEFINE_REG2;

extern ST_CUS_DEFINE_REG2 st_reg_cus_def_reg2;

#ifndef CUS_DEF_REG3_LEN
#define CUS_DEF_REG3_LEN 2
#endif

typedef struct{
	u8 data[CUS_DEF_REG3_LEN];
	u8 r_count;
	u8 w_count;

} ST_CUS_DEFINE_REG3;

extern ST_CUS_DEFINE_REG3 st_reg_cus_def_reg3;

#ifndef CUS_DEF_REG4_LEN
#define CUS_DEF_REG4_LEN 2
#endif

typedef struct{
	u8 data[CUS_DEF_REG4_LEN];
	u8 r_count;
	u8 w_count;

} ST_CUS_DEFINE_REG4;

extern ST_CUS_DEFINE_REG4 st_reg_cus_def_reg4;

#ifndef CUS_DEF_REG5_LEN
#define CUS_DEF_REG5_LEN 2
#endif

typedef struct{
	u8 data[CUS_DEF_REG5_LEN];
	u8 r_count;
	u8 w_count;

} ST_CUS_DEFINE_REG5;

extern ST_CUS_DEFINE_REG5 st_reg_cus_def_reg5;


///////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For Edge Event */
#define EDGE_EVENT_LEN			(2 + SA_COUNT)

typedef struct{
	u8 data[EDGE_EVENT_LEN];
	u8 r_count;
	u8 w_count;

} ST_EDGE_EVENT;

extern ST_EDGE_EVENT st_reg_edge_event;

///////////////////////////////////////////////////////////////////////////////////////////////////

typedef enum{
	EM_ADC = 0x0,
	EM_DAC,
	EM_RAW_DATA,
	EM_BASE_LINE,

	EM_SIG_VALUE,
	EM_FORCE_VALUE,
	EM_TOUCH_VALUE,
	EM_POSITION,
	EM_EVENT,

	EM_FORCE_FLAG,
	EM_TOUCH_FLAG,
	EM_POSITION_FLAG,
	EM_EVENT_FLAG,

	EM_OTHER_DEBUG_VALUE,

	EM_OUTPUT_END,

	EM_OUTPUT_ERROR = 0xFF,
} EM_OUTPUT;

extern EM_OUTPUT em_output;

typedef struct{
	char head;
	char sub_head;

	u8 r_count;
	u8 w_count;

	AdcType *data_ptr;
} ST_OUTPUT;

extern ST_OUTPUT st_output[EM_OUTPUT_END];

typedef struct{
	u8 r_count;
	u8 w_count;
	u8 parameter;

} ST_OUTPUT_OPT;

extern ST_OUTPUT_OPT st_output_opt[EM_OUTPUT_END];

//////////////////////////////////////////////////////////////////////////////////////////

typedef struct{

	AdcType adc_data[PHY_CH_COUNT];
	DacType dac_data[PHY_CH_COUNT];
	AdcType debug_data[DEBUG_COUNT];

	s16   temprature_init;
	s8    temprature_init_bat;
	float temprature_cur_aw;
	float temprature_cur;
	float temprature_real;
	float temp_coef_buf[PHY_CH_COUNT];
	float ap_adjust_coef_buf[CH_COUNT];
	float balance_buf[PHY_CH_COUNT];
	float hol_balance_buf[CH_COUNT];    //holistic_balance_buf
	AdcType model_buf[TOTAL_MODEL_CNT];

	AdcType model_df_buf[TOTAL_MODEL_DF_CNT]; //model of double finger

#if (defined(ACTUAL_FORCE_CALC_EN) && ACTUAL_FORCE_CALC_EN)
	AdcType model_inv_buf[TOTAL_MODEL_INV_CNT];
#endif

	AdcType origin_adc_data[PHY_CH_COUNT];
} ST_COMMON;

extern ST_COMMON st_common;

//////////////////////////////////////////////////////////////////////////////////////////

typedef struct{

	u8 ch_start[GR_COUNT];
	u8 ch_count[GR_COUNT];
	u8 ch_end[GR_COUNT];

	u8 bu_start[GR_COUNT];
	u8 bu_count[GR_COUNT];
	u8 bu_end[GR_COUNT];

	u16 model_start[GR_COUNT]; //model start offset
	u16 model_count[GR_COUNT]; //preset model length, determined by presetting
	u16 model_end[GR_COUNT];

	u16 model_df_start[GR_COUNT]; //double finger model start offset
	u16 model_df_count[GR_COUNT]; //preset model length, determined by presetting
	u16 model_df_end[GR_COUNT];

	u16 model_inv_start[GR_COUNT];

	AdcType *data_ptr;
	float *hol_balance_ptr; //balance_value * temprature value
	AdcType *model_ptr;

} ST_SENSOR;

extern ST_SENSOR st_sensor;


//////////////////////////////////////////////////////////////////////////////////////////

//related to int pin 	
#define REG_IDLE  0X00
#define REG_BUSY_KEY  0X01
#define REG_BUSY_RST  0X02
#define REG_BUSY_CH_ERR  0X04
#define REG_BUSY_LIGHT_TOUCH   0X08
#define REG_BUSY_SLIDE   0X10

//////////////////////////////////////////////////////////////////////////////////////////

#endif  /* __DATA_TABLE_H */
