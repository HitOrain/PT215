/*************************************************
* VIVO NEX V1 - USER PARAMETER
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "compile_option.h"

#if (VIVO_AW_8686X_PT215_C1V1_UG_V1)

#ifndef __VIVO_AW_8686X_PT215_C1V1_UG_V1_H
#define __VIVO_AW_8686X_PT215_C1V1_UG_V1_H

#include "customer_common.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define For DATA TABLE */

#define PROJECT_ID			215

#define CUS_MAIN_VER		4
#define CUS_SUB_VER			13

/* Define For NDT Sensors */

#define CH_G1			1
#define MODEL_G1		0
#define BU_G1			1
#define MODEL_DF_G1     0

#define CH_G2			1
#define MODEL_G2		0
#define BU_G2			1
#define MODEL_DF_G2     0

#define TCC_TABLE_MAX   7

#define GR_COUNT        2
#define CH_COUNT		(CH_G1 + CH_G2)
#define BU_COUNT		(BU_G1 + BU_G2)
#define SA_COUNT		1
#define PHY_CH_COUNT    2
#define TEMP_CH_COUNT    1

#define LAST_LARGE_MODEL_ID 	15
#define CONST_LARGE_MODEL_LEN   250

#define TOTAL_MODEL_CNT		(1)
#define TOTAL_MODEL_DF_CNT  (1)
#define TOTAL_MODEL_INV_CNT (1)

#define MAX_MODEL_COUNT		1
#define MAX_CH_SUB_CNT      CH_G1  //single group's max ch num

#define ACTUAL_FORCE_CALC_EN 0
#define DBG_POS_COS_SHOW_EN  0
// #define MEDIAN_FILTER


#define DF_SCAN_MODE_SELECT	1
#define DF_FUNCTION_SWITCH_SELECT 1  //button function enable , slide function disable

//oppo need to report result to android by fw 20200513
#define EN_RESULT_CALC_BY_FW
#define NOISE_LEN (200)
#define OFFSET_MAX_THR (220) //unit:mv
#define OFFSET_MIN_THR (-220)
#define NOISE_STD_MAX_THR (33) //unit:uv
#define NOISE_STD_MIN_THR (1)
#define NOISE_PEAK_MAX_THR (100)
#define NOISE_PEAK_MIN_THR (2)

#ifdef EN_RESULT_CALC_BY_FW
    extern void dbg_mode_read_offset_res(u8 mode_select);
    extern void dbg_mode_read_chip_noise_res(u8 mode_select);
    extern void dbg_mode_read_noise_res(u8 mode_select);
#endif

#define DBG_MODE_SELF_TEST_AUTO_TOGGLE_IO_EN  //enable io auto toggle when self test happens
//#define F61_DAC_SCAN_EN                       //F61 DAC SCAN ONLY NEED USED IN PT100 & PT101
#define EN_NOISE_CALC_BY_FW

#define EN_OUTP_SINGLE_FORCE
#ifdef EN_OUTP_SINGLE_FORCE
    extern void output_single_force(u8 i);
#endif

extern void dbg_mode_read_noise_val(u8 mode_select);

#define CUS_DEF_REG2_LEN 16 //tp limit : x1 y1 x2 y2
#define CUS_DEF_REG4_LEN 10 //tp info : x1 y1 x2 y2

#define EN_RST_BASE_FROM_HISTORY_BASE 1
#define REL_DIS_BASE_TRACKING_CNT 1

#define OFFSET_CANCEL_STABLE_THR  31  //when adc diff bigger than this value , disable offset cancel for a moment
//#define KNOCK_DET_THR     300   //each ms max adc delt val
//#define KNOCK_DET_THR     3000      //walt1207
#define KNOCK_DET_THR     500
#define DEFALUT_INIT_TEMP_VAL 30

#define HIGH_TEMP_THR    10

#define DF_HIGH_SPEED		6
#define DF_LOW_SPEED		1000
#define DF_POWER_OFF		1000
#define DF_DEEP_SLEEP_SPEED 1000
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* The parameter of DATA TABLE */
#define MD_COUNT			8
#define GA_COUNT			2
#define RST_SRC_COUNT       4
///////////////////////////////////////////////////////////////////////////////
#define MCU_TM0_REG_VAL (DF_HIGH_SPEED)


/* Define For NDT Algorithm */

//#define SAMPLE_COUNT		50
#define SAMPLE_COUNT		100

#define AVG_POWER_COUNT		5
#define AVG_POWER_COUNT_IDLE 2

#define BASE_LINE_NARROW	250
#define BASE_LINE_WIDTH		400

#define ADC_BIT_SHIFT 2
#define ADC_RANG_ADJUST 2048
#define OFFSET_CANCEL_BASE_LINE 848

//////////////////////force flag//////////////////////////////
#define FF_TRI_TH	    (50)	// force flag diff threshold
#define FF_REL_TH       (25)
#define FF_LOCAL_TH     (FF_TRI_TH)

#define FF_SLP_TH1	    (3)	   // force flag slp threshold
#define FF_SLP_TH2	    (3)	   // force flag slp threshold
#define FF_LEAVE_COEF	(0.2)

//////////////////////baseline//////////////////////////////
#define BASELINE_SALPHA		(5)	//0.1*1024
#define BASE_CHANGE_LIMIT   (1 << 8)
#define ORDER16_BASE        (4)
#define ORDER16             (1 << ORDER16_BASE)

//////////////////////touchflag//////////////////////////////
#define TF_TRI_TH     (50)
#define TF_REL_TH      (30)
#define TF_REL_COEF    (0.3)
#define TF_LOCAL_TH    TF_TRI_TH
#define TF_REL_MIN_SLP_TH (-6)

//////////////////////force value///////////////////////////
//#define LOCAL_BUFF_LENG    100
#define LOCAL_BUFF_LENG    80

#define FORCE_STABLE_SUM_TH1  (ORDER16 * ORDER16 << 1)
#define FORCE_STABLE_PRD_TH1  (20)
#define FORCE_STABLE_CUR_TH1  (10)
#define FORCE_STABLE_SUM_TH2  (ORDER16 * ORDER16)
#define FORCE_STABLE_PRD_TH2  (20 >> 1)
#define FORCE_STABLE_CUR_TH2  (7)
#define FORCE_STABLE_SUM_TH3  (ORDER16 * ORDER16 >> 1)
#define FORCE_STABLE_PRD_TH3  (20 >> 1)
#define FORCE_STABLE_CUR_TH3  (5)

//////////////////////pos detect///////////////////////////

#define SINGLE_POS_VALID_THRS (SQUARE(0.90)*((s32)1<<16))
#define SINGLE_POS_VALID_THRM (SQUARE(0.95)*((s32)1<<16))

#define DOUBLE_POS_VALID_THRS (SQUARE(0.70)*((s32)1<<16)) //SMALL THR
#define DOUBLE_POS_VALID_THRM (SQUARE(0.80)*((s32)1<<16)) //MIDDLE THR

#define SINGLE_SIM_CALC_RANGE  (35)


//////////////////////temprature monitor/////////////////////////////
#define TEMPRATURE_MONITOR_INTERVAL  2000
#define TIME_OUT_DEFAULT_THR 30000
#define TIME_OUT_UPPER_LIMIT_THR 30000
#define TIME_OUT_LOWER_LIMIT_THR 6000

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

#define TRI_THR_DOUBLE_FINGER   200

#define TRI_THR_BU1		250//150//60
#define REl_THR_BU1		150//48

#define TRI_THR_BU2		250//150//60
#define REl_THR_BU2		150//48

#define TRI_THR_BU1_HEAVY		300//100
#define REl_THR_BU1_HEAVY		180//60

#define TRI_THR_BU2_HEAVY		300//100
#define REl_THR_BU2_HEAVY		180//60


/////////////////////////////////////////////////////////////////////

#define POS_START_BU1		2
#define POS_END_BU1		10
#define POS_START_BU2		24
#define POS_END_BU2		32
#define POS_START_BU3		50
#define POS_END_BU3		60

#define POS_START_BU1_WIDE		1
#define POS_END_BU1_WIDE		13
#define POS_START_BU2_WIDE		22
#define POS_END_BU2_WIDE		34
#define POS_START_BU3_WIDE		49
#define POS_END_BU3_WIDE		61


/////////////////////////////////////////////////////////////////////

#define CH_START_BU1		0
#define CH_END_BU1		0
#define CH_START_BU2		1
#define CH_END_BU2		1
#define CH_START_BU3		2
#define CH_END_BU3		2

/////////////////////////////////////////////////////////////////////

#define TRI_THR_SA1		40
#define REL_THR_SA1		30


/////////////////////////////////////////////////////////////////////

/* Define for DEBUG */

//#define DBG_COLLECT_ADC

#define DEBUG_COUNT		1

#define UART_LOG		0

/////////////////////////////////////////////////////////////////////

#if (UART_LOG)

#else

#endif

/////////////////////////////////////////////////////////////////////
/* Define for DEFAULT OUT DATA OPTION */

#define ADC_VALUE		0
#define DAC_VALUE		0
#define RAW_DATA		1
#define BASE_LINE_VALUE	0

#define SIG_VALUE		1
#define FORCE_VALUE		1
#define TOUCH_VALUE		0
#define POSITION_VALUE          0
#define EVENT_VALUE             0

#define FORCE_FLAG              0
#define TOUCH_FLAG              0
#define POSITION_FLAG           0
#define EVENT_FLAG              0

#define OTHER_DEBUG_VALUE       0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define for Platform */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CHANNEL_POWER_SWITCH

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define for GPIO Pin */
#define GPIO_OTHERS			1(GPIO_PA2 | GPIO_PA3/*| GPIO_PA8 | GPIO_PA9 | GPIO_PA10 | GPIO_PA11*/)
#define GPIO_POWER			1(GPIO_PA0 | GPIO_PA1)

//#define GPIO_AP2MCU_INT		(GPIO_PA7)   //keep low when power off ; keep high when power on

#define GPIO_VOICE_UP       7//(GPIO_PA7)
#define GPIO_VOICE_DOWN     8//(GPIO_PA8)
#define GPIO_MCU2AP_INT		6//(GPIO_PA6)
							
#define GPIO_MOTOR_UP       9//(GPIO_PA9)
#define GPIO_MOTOR_DOWN     10//(GPIO_PA10)
#define MOTOR_COUNT			(2)

#define GP_IN_COUNT		0
#define GP_OUT_COUNT	3
#define GP_COUNT        (GP_IN_COUNT + GP_OUT_COUNT)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define TIMER_COUNT		1
#define AFE_COUNT		1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define for MCU Vendor */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define AFE_GAIN1  			64
#define AFE_GAIN2  			8

//PGA1=64
//dac2uv = (0.614*2800000/3)/2^(11-1) = 560uv/dac
//aw_adc2uv = 2800000/512/2^13 = 0.6675uv/adc
//adc2uv = 2800000/512/2^11 = 2.67uv/adc

#define DAC_2_ADC_COEF      209

/* The parameter of AFE */
#define AFE_INT_EN				ENABLE	//Disable AFE interrupt at start
#define AFE_PGA_EN				ENABLE
#if (64 == AFE_GAIN1)
    #define AFE_PGA1_GAIN			AFE_PGA1_Gain_64
#else
    #define AFE_PGA1_GAIN			AFE_PGA1_Gain_128
#endif
#define AFE_PGA2_GAIN			AFE_PGA2_Gain_8
#define AFE_SAMPLE_TIME			Time_ADCClock_32

#define AFE_AVG_MODE			Mid_Avg	//All_Avg or Mid_Avg
#define AFE_CYCLE_MODE1_UVALID_NUM	7	//
#define AFE_CYCLE_MODE_VALID_NUM	Vaild_Num_16
#define AFE_SCAN_MODE			AFE_FiniteCycleMode1;
#define AFE_VS_SEL				VS_28	//VS output 2.8V

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* The parameter of IIC */
#define IIC_SDA				Reuse_PA13
#define IIC_SCL				Reuse_PA12

#define IIC_ADDR			0x50	//I2CSLAVE_ADDR;
#define IIC_RX_FIFO_LEVEL		1		//once one byte received,interrupt generated
#define IIC_TX_FIFO_LEVEL		8


#define USE_MIXED_RAW                  //algorithm rawdata is mixed by adc val, such as twist channel.
extern s16 raw_reshape(u8 i);
extern void raw_overflow_check_indv(void);


#define CONFIG_LENG_B  (128)
#define CONFIG_LENG_W  (CONFIG_LENG_B / 2)

#define TCC_TABLE_UPDATE_FUNCTION_SUPPORT

typedef struct
{
    s16 button_tri_thr[BU_COUNT];  //0~1
    s16 button_rel_thr[BU_COUNT];  //2~3
    s16 repair_mode;                     //4
    s16 modify_times;						//5
    u16 tp_x_min[BU_COUNT];			//6~7
    u16 tp_x_max[BU_COUNT];			//8~9
    u16 tp_y_min[BU_COUNT];			//10~11
    u16 tp_y_max[BU_COUNT];			//12~13
    u16 motor_mod[BU_COUNT];		//14-15
    s16 res[CONFIG_LENG_W - 16 - 1];     //16~64
    s16 checksum;                        //65
} ST_CHIP_CONFIG;

extern ST_CHIP_CONFIG st_config;

#define USE_DEFAULT_MODEL
#define USE_UG_FLASH_MODEL
#ifdef USE_UG_FLASH_MODEL
    extern void init_ug_model_from_flash(void);
#endif

// #define EN_WRITE_BUTTON_THR_2_FLASH
extern void write_button_thr_2_flash(void);
void init_tcc_table_poweron(void);

#define MAX_TP_COUNT 2
#define MAX_SENSOR_COUNT 2
#define MAX_MODELCOUNT 250 //375?

typedef struct TP_POINT
{
    s16 x_point;
    s16 y_point;
    u16 rel_cnt;
    u16 tri_cnt;
    u16 x_start;
    u16 y_start;
    u16 x_min;
    u16 x_max;
    u16 y_min;
    u16 y_max;
    u8 state; 		//0: no point    1: valid point
    u8 x_delta;
    u8 y_delta;
    u8 x_count;
    u8 y_count;
} ST_TP_POINT;

extern ST_TP_POINT st_tp_info[MAX_TP_COUNT];

#define SCREEN_INFO_SUPPORT

typedef enum
{
    PHY_CHANNEL_0 = 0x00,
    PHY_CHANNEL_1,
    PHY_CHANNEL_2,
    PHY_CHANNEL_3,
    PHY_CHANNEL_4,
    PHY_CHANNEL_5,
    PHY_CHANNEL_6,
    PHY_CHANNEL_7,
} Sensor_Phy_Channel_TypeDef;

typedef enum
{
    USE_CHANNEL_ADD = 1,
    USE_CHANNEL_SUB = -1,
    NOT_USE_CHANNEL = 0,
} Channel_Combine_Method_TypeDef;

typedef enum
{
    LOGIC_CHANNEL_0 = 0x00,
    LOGIC_CHANNEL_1,
    LOGIC_CHANNEL_2,
    LOGIC_CHANNEL_3,
    LOGIC_CHANNEL_4,
    LOGIC_CHANNEL_5,
    LOGIC_CHANNEL_6,
    LOGIC_CHANNEL_7,
} Sensor_Logic_Channel_TypeDef;

#define MAX_NUM_OF_PHY_IN_A_LOGIC_CHANNEL 2
typedef struct CHANNEL_PHY_TO_LOGIC_MAP
{
    Sensor_Phy_Channel_TypeDef phy_to_logic_channel_map[CH_COUNT][MAX_NUM_OF_PHY_IN_A_LOGIC_CHANNEL];
    Channel_Combine_Method_TypeDef com_method[CH_COUNT][MAX_NUM_OF_PHY_IN_A_LOGIC_CHANNEL];

} CHANNEL_PHY_TO_LOGIC_MAP_S;


#define MAX_NUM_OF_LOGIC_CHANNEL_FOR_A_BUTTON 2
typedef struct LOGIC_CHANNEL_TO_BUTTON_MAP
{
    Sensor_Logic_Channel_TypeDef channel_to_button_map[BU_COUNT][MAX_NUM_OF_LOGIC_CHANNEL_FOR_A_BUTTON];

} LOGIC_CHANNEL_TO_BUTTON_MAP_S;

typedef struct
{
    float temp;
    const s8 (*p_model)[MAX_MODELCOUNT];
} TypeDef_Temp_Model;

extern const TypeDef_Temp_Model temperature_model[TCC_TABLE_MAX];
#endif
#endif

