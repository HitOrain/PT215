/*************************************************
* VIVO DEMO ON NEX PLATFORM USING CS_61 MCU
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#include "compile_option.h"

#if (VIVO_AW_8686X_PT215_C1V1_UG_V1)

#include "hal_ndt_common.h"
#include "math.h"
#include "algorithm_init.h"

// #define NOT_USE_TP
////////////////////////////////////////////////////////////////////////////////////////////////////
ST_HAL_PLATFORM st_hal_platform =
{
//    {AINP2, AINP0,  AINP4,  AINP6},

	{2, 0},
	
//    {GPIO_VOICE_UP, GPIO_VOICE_DOWN, GPIO_MCU2AP_INT},
    {1, 2, 3},

    0x00,
    #ifdef CHANNEL_POWER_SWITCH
//    {VS_PA0, VS_PA0, VS_PA1, VS_PA1},
	{0, 1},
    #endif
    
    {false},
    {false},
    {0, 0},
    0,
    0
};

u16 motor_io[MOTOR_COUNT] = {GPIO_MOTOR_UP, GPIO_MOTOR_DOWN};

#endif

#define TCC_TABLE_MAX 7

typedef struct
{
    float temp;
    float coeff[CH_COUNT];
} TypeDef_TCC;

const TypeDef_TCC TCC_Table_Default[TCC_TABLE_MAX] =
{
//    {-10,	2.3644,	1.8795,	2.9845,	4.4914},
//    {0,		1.4199,	1.1856,	1.8625,	2.1888},
//    {10,	1.0553,	0.9379,	1.3581,	1.3124},
//    {20,	0.9874,	0.9418,	1.0906,	1.1284},
//    {30,	1,	1,	1,	1},
//    {40,	1.0334,	1.0553,	0.9724,	0.9287},
//    {50,	1.0914,	1.133,	0.9913,	0.9239},
    
    {-10,	{1,	1}},
    {0,		{1,	1}},
    {10,	{1,	1}},
    {20,	{1,	1}},
    {30,	{1,	1}},
    {40,	{1,	1}},
    {50,	{1,	1}},
};

TypeDef_TCC TCC_Table[TCC_TABLE_MAX];

ST_COMMON st_common =
{
    {0},
    {0},
    {0},
    25, 25, 25, 25, 25,
    {1, 1},

    {1, 1},

    {1, 1},

    {0},

    {0},
};

ST_CHANNEL_GAP st_reg_channel_gap =
{
    {50, 0},
    CHANNEL_GAP_LEN,
    0,
};

/////////////////////////////////////////////////////////////////////////////////////////

ST_SENSOR st_sensor =
{
    {0, CH_G1},
    {CH_G1, CH_G2},
    {CH_G1, (CH_G1 + CH_G2)},

    /////////////////////////////////////////////////////////////

    {0, BU_G1},
    {BU_G1, BU_G2},
    {BU_G1, (BU_G1 + BU_G2)},

    /////////////////////////////////////////////////////////////

    {0},
    {0},
    {0},

    /////////////////////////////////////////////////////////////

    {0},
    {0},
    {0},

    {0},

    /////////////////////////////////////////////////////////////

    st_common.adc_data,
    st_common.hol_balance_buf,
    st_common.model_buf,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_ALGO_PARA st_algo_para =
{
    {TRI_THR_BU1,		TRI_THR_BU2},
    {REl_THR_BU1,		REl_THR_BU2},

    {POS_START_BU1,		POS_START_BU2},
    {POS_END_BU1,		POS_END_BU2},

    {POS_START_BU1_WIDE, POS_START_BU2_WIDE},
    {POS_END_BU1_WIDE,  POS_END_BU2_WIDE},

    {CH_START_BU1,		CH_START_BU2},
    {CH_END_BU1,		CH_END_BU2},

    ///////////////////////////////////////////////////////////////////////////////////////////

    {TRI_THR_SA1/*,		TRI_THR_SA2*/},
    {REL_THR_SA1/*,		REL_THR_SA2*/},

};

ST_BUTTON_FORCE_THR st_reg_button_force_thr =
{
    {
        SPILT_TWO_BYTE(TRI_THR_BU1), SPILT_TWO_BYTE(REl_THR_BU1),
        SPILT_TWO_BYTE(TRI_THR_BU2), SPILT_TWO_BYTE(REl_THR_BU2),
    },

    BUTTON_FORCE_THR_LEN,
    BUTTON_FORCE_THR_LEN,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_BUTTON_POS st_reg_button_pos =
{
    {
        SPILT_TWO_BYTE(POS_START_BU1), SPILT_TWO_BYTE(POS_END_BU1),
        SPILT_TWO_BYTE(POS_START_BU2), SPILT_TWO_BYTE(POS_END_BU2),
    },

    BUTTON_POS_LEN,
    BUTTON_POS_LEN,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_SLIDE_FORCE_THR st_reg_slide_force_thr =
{
    {
        SPILT_TWO_BYTE(TRI_THR_SA1), SPILT_TWO_BYTE(REL_THR_SA1),
    },

    SLIDE_FORCE_THR_LEN,
    SLIDE_FORCE_THR_LEN,
};

///////////////////////////////////////////////////////////////////////////////

ST_ADC_2_UVOLT_COEF st_reg_adc_2_uvolt_coef =
{
    {0x4c, 0x68, 0},           // (2.8 * 1000000 (V -> uV) / 2048 (ADC range) / 512 (AFE Gain)), (x10000)  = 2.67 * 10000
    ADC_2_UVOLT_COEF_LEN,
    0,
};

ST_DAC_2_UVOLT_COEF st_reg_dac_2_uvolt_coef =
{
    {0x30, 0x02},                //dac2uv = (0.614*2800000/3)/2^11-1 = 560uv/dac
    DAC_2_UVOLT_COEF_LEN,
    0,
};

ST_GROUP_MAP_INFO st_reg_group_map_info =
{
    {CH_G1,	MODEL_G1,},
    GROUP_MAP_INFO_LEN,
    0,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_CUS_PARA st_cus_para =
{
    false,
    false,

	DF_HIGH_SPEED,
	DF_LOW_SPEED,
	DF_DEEP_SLEEP_SPEED,
	DF_POWER_OFF,
};

ST_CHIP_CONFIG st_config =
{
    {TRI_THR_BU1, TRI_THR_BU2}, //BU_tRI
    {REl_THR_BU1, REl_THR_BU2}, //BU_REL
    0,
    0,
    {0, 0}, //tp_x_min
    {MAX_U16, MAX_U16}, //tp_x_max
    {0, 0}, //tp_Y_min
    {MAX_U16, MAX_U16}, //tp_Y_max
    {4, 4}, //motor mode
};


ST_TP_POINT st_tp_info[MAX_TP_COUNT];

s16 model[MAX_SENSOR_COUNT][MAX_MODELCOUNT];

const s16 sub_model_1[MAX_SENSOR_COUNT][MAX_MODELCOUNT] =
{
    {
		7407,    5212,    4384,    4324,    5180,    6973,    7299,    5359,    4356,    4509,   6224,   6224,
		3766,    2543,    1986,    1755,    1811,    2129,    2208,    1886,    1815,    2109,   3017, 	 3017,
		2557,    1698,    1249,     998,     889,     918,     943,     892,     989,    1275,   1904,   1904,
		2046,    1333,     948,     703,     541,     444,     434,     523,     695,     978,   1513,   1513,
		1953,    1281,     916,     673,     500,     383,     369,     470,     649,     926,   1439,   1439,
		2174,    1452,    1075,     844,     715,     675,     679,     711,     853,    1133,   1674,   1674,
		2750,    1919,    1503,    1314,    1313,    1540,    1669,    1468,    1463,    1741,   2439,   2439,
		4076,    2988,    2559,    2543,    3011,    4132,    4699,    3698,    3134,    3253,   4226,   4226,
		6917,    5429,    5208,    6320,   10948,   30000,   30000,   25591,   11697,    9375,  10782,  10782,
	   15488,   14384,   20034,   30000,  -21187,  -11012,   -9533,  -12193,  -23928,  -30000,  30000,  30000,
	   30000,   30000,  -30000,  -13967,   -8582,   -6705,   -6317,   -7032,   -9112,  -13559, -22354, -22354,
	  -30000,  -24672,  -13662,   -8881,   -6750,   -5913,   -5720,   -6286,   -7679,  -10469, -15720, -15720,
	  -30000,  -18402,  -11435,   -8228,   -6598,   -5931,   -5822,   -6338,   -7480,   -9969, -14937, -14937,
	  -29750,  -16651,  -11070,   -8268,   -6948,   -6383,   -6329,   -6929,   -8197,  -10663, -15952, -15952,
    },
    {
		3989,    2558,    1968,    1679,    1574,    1621,    1834,    2243,    2988,    4335,   7475,   7475,
		3037,    1925,    1464,    1245,    1179,    1229,    1422,    1787,    2423,    3576,   6150,   6150,
		2275,    1436,    1083,     920,     882,     947,    1139,    1502,    2137,    3294,   5841,   5841,
		1907,    1159,     841,     699,     673,     743,     933,    1284,    1900,    3026,   5629,   5629,
		1687,     979,     673,     536,     516,     598,     810,    1207,    1928,    3291,   6513,   6513,
		1746,     934,     583,     425,     399,     498,     747,    1206,    2026,    3609,   7746,   7746,
		1950,     975,     550,     341,     307,     449,     795,    1390,    2464,    4752,  12085,  12085,
		2252,    1102,     590,     300,     253,     443,     878,    1527,    2716,    5232,  14643,  14643,
		2117,    1042,     567,     315,     272,     448,     862,    1530,    2770,    5543,  15727,  15727,
		1860,     960,     567,     384,     356,     475,     777,    1332,    2348,    4429,  11073,  11073,
		1703,     952,     629,     486,     467,     567,     819,    1301,    2196,    4050,   9038,   9038,
		1785,    1077,     767,     636,     620,     713,     942,    1373,    2153,    3644,   7448,   7448,
		2049,    1308,     986,     853,     843,     943,    1185,    1634,    2438,    3984,   7614,   7614,
		2688,    1773,    1383,    1218,    1204,    1325,    1609,    2110,    3010,    4692,   8787,   8787,
		2688,    1773,    1383,    1218,    1204,    1325,    1609,    2110,    3010,    4692,   8787,   8787,
    },
};

CHANNEL_PHY_TO_LOGIC_MAP_S pt154_chnl_phy_to_logic_map =
{
    {
        {PHY_CHANNEL_0, PHY_CHANNEL_0},
        {PHY_CHANNEL_1, PHY_CHANNEL_1},
//        {PHY_CHANNEL_2, PHY_CHANNEL_2},
//        {PHY_CHANNEL_3, PHY_CHANNEL_3},
    },

    {
        {USE_CHANNEL_ADD, NOT_USE_CHANNEL},
        {USE_CHANNEL_ADD, NOT_USE_CHANNEL},
//        {USE_CHANNEL_ADD, NOT_USE_CHANNEL},
//        {USE_CHANNEL_ADD, NOT_USE_CHANNEL},
    },
};

LOGIC_CHANNEL_TO_BUTTON_MAP_S pt154_chnl_to_button_map =
{
    {
        {LOGIC_CHANNEL_0, LOGIC_CHANNEL_1},
        {LOGIC_CHANNEL_2, LOGIC_CHANNEL_3},
    },
};

SCREEN_INFO_S m_screen_info =
{
    1440,
    3200,

    70.697,
    157.1,

    29,
    12,

    10.35,
    11.05,
};

void tp_info_init(void)
{
    int i;

    for (i = 0; i < MAX_TP_COUNT; i++)
    {
        st_tp_info[i].state = 0;
        st_tp_info[i].x_delta = 102;
        st_tp_info[i].y_delta = 102;

        st_tp_info[i].x_min = 0;//st_config.tp_x_min[i];
        st_tp_info[i].x_max = MAX_U16;//st_config.tp_x_max[i];
        st_tp_info[i].y_min = 0;//st_config.tp_y_min[i];
        st_tp_info[i].y_max = MAX_U16;//st_config.tp_y_max[i];
    }

    st_tp_info[0].x_count = 12;
    st_tp_info[0].y_count = 14;
    st_tp_info[0].x_start = 160;
    st_tp_info[0].y_start = 174;
    st_tp_info[1].x_count = 12;
    st_tp_info[1].y_count = 15;
    st_tp_info[1].x_start = 160;
    st_tp_info[1].y_start = 1651;

    for (i = 0; i < MAX_SENSOR_COUNT; i++)
    {
        memcpy(&model[i][0], &sub_model_1[i][0], sizeof(sub_model_1[0]));
    }
}

void init_tcc_table_poweron(void)
{
    memcpy(TCC_Table, TCC_Table_Default, sizeof(TCC_Table));
}

#ifdef USE_UG_FLASH_MODEL
//will be call by hal_flash_para_init()
void init_ug_model_from_flash()
{
    //note: here CONST_LARGE_MODEL_LEN != MAX_MODELCOUNT
    int i;

    for (i = 0; i < MAX_SENSOR_COUNT; i++)
    {
        init_large_model_from_flash((AdcType *)&model[i][0], 2 * i, CONST_LARGE_MODEL_LEN);
        init_large_model_from_flash((AdcType *)&model[i][CONST_LARGE_MODEL_LEN / 2], 2 * i + 1, CONST_LARGE_MODEL_LEN);
    }
}
#endif

#define MAX_MODEL_LIMIT (10000)
#define BIG_CALI_THR (9)

s16 interp_quadratic(u16 coordinate_x, u16 coordinate_y, u8 sensor_id, s16 model_buf[][MAX_MODELCOUNT])
{
#define X_INTERVAL (10)
#define Y_INTERVAL (10)

//    u8 tp_id = sensor_id / 2;

	u8 tp_id = sensor_id;	//hzf

    u16 coor_x = coordinate_x * X_INTERVAL / st_tp_info[tp_id].x_delta;
    u16 coor_y = coordinate_y * Y_INTERVAL / st_tp_info[tp_id].y_delta;
    u16 coor_x0 = st_tp_info[tp_id].x_start * X_INTERVAL / st_tp_info[tp_id].x_delta;
    u16 coor_y0 = st_tp_info[tp_id].y_start * Y_INTERVAL / st_tp_info[tp_id].y_delta;

    // 计算插值区间的index起点
    s16 i = (coor_x - coor_x0) / X_INTERVAL;
    s16 j = (coor_y - coor_y0) / Y_INTERVAL;

    i = MAX(0, i);
    j = MAX(0, j);
    i = MIN(i, st_tp_info[tp_id].x_count - 2);
    j = MIN(j, st_tp_info[tp_id].y_count - 2);

    // 坐标按起点归0�?
    s16 cx = coor_x - i * X_INTERVAL - coor_x0;
    s16 cy = coor_y - j * Y_INTERVAL - coor_y0;

    // 超过范围则按边缘值计�?
    cx = MAX(0, cx);
    cy = MAX(0, cy);
    cx = MIN(cx, X_INTERVAL);
    cy = MIN(cy, Y_INTERVAL);

    s32 fit_val = 0;
    s32 fit_val_x;

    for (u8 yk = 0; yk < 2; yk++)
    {
        u8 yk1 = (yk + 1) % 2;

        s16 mo1 = model_buf[sensor_id][i + 0 + (j + yk) * st_tp_info[tp_id].x_count];
        s16 mo2 = model_buf[sensor_id][i + 1 + (j + yk) * st_tp_info[tp_id].x_count];

        // mo1 = MIN(MAX_MODEL_LIMIT, mo1); //模板限幅
        // mo2 = MIN(MAX_MODEL_LIMIT, mo2);

		//prevent divisor by zero  walt0310+
        {
            if ((mo1 >= - 1) && (mo1<= 1))
            {
                mo1 =1;
            }

            if ((mo2 >= - 1) && (mo2 <= 1))
            {
                mo2 =1;
            }
        }
		
        mo1 = 100000 / mo1; //转换�?00g对应的信号量
        mo2 = 100000 / mo2; //转换�?00g对应的信号量

        fit_val_x = mo1 * (X_INTERVAL - cx) \
                    + mo2 * (cx);

        fit_val += fit_val_x * (cy - yk1 * Y_INTERVAL) / (yk - yk1); // y方向插�?
    }

    if (0 == fit_val)
    {
        return MAX_MODEL_LIMIT;
    }

    fit_val = 10000000 / fit_val; //因为X_INTERVAL  X_INTERVAL都是10 所以这里直接简化了

    fit_val = MIN(fit_val, MAX_MODEL_LIMIT); //限幅
    fit_val = MAX(fit_val, -MAX_MODEL_LIMIT); //限幅

    return fit_val;
}

float get_tp_coef(u8 i)
{
    static u8 tp_keep_cnt[GR_COUNT];
    s16 ret;
	
//    u8 tp_index = i / 2;

	u8 tp_index = i;	//hzf

    //离手的前3帧用原来的系�?
    if (!st_tp_info[tp_index].state)
    {
        if (tp_keep_cnt[tp_index] < 3 * 2) //每个周期该变量会自加2�?监控3各周�?
        {
            tp_keep_cnt[tp_index]++;
            return st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];
        }
    }

    if (i > MAX_SENSOR_COUNT - 1 || !st_tp_info[tp_index].state)
    {
    	if(st_hal_platform.ap_status == 2){
        	return 0.2;
    	} else {
			return 0.5;
		}
    }

    tp_keep_cnt[tp_index] = 0;

    ret = interp_quadratic(st_tp_info[tp_index].x_point, st_tp_info[tp_index].y_point, i, model);
    // ret = MIN(5000, ret);
    return (float)ret / 1000;

}

//////////////////////////////////////////////////////////////////////////////////////////

void hal_platform_init(void)
{
    u8 i;
	
    st_hal_platform.gpio_ap2mcu = st_reg_gpio_status.data[3];

    st_reg_edge_event.data[0] |= REG_BUSY_RST | REG_BUSY_CH_ERR;

    st_hal_platform.ap_status |= AP_STATUS_GAME_MODE;  // rst default in game mode
    
    st_reg_ap_status.data[0] = st_hal_platform.ap_status;

    st_reg_cus_def_reg1.data[0] = 10;
    st_reg_cus_def_reg1.data[1] = 10;

    st_reg_noise_thr.data[0] = NOISE_LEN;
    st_reg_noise_thr.data[1] = NOISE_LEN >> 8;
    st_reg_noise_thr.data[2] = NOISE_PEAK_MIN_THR * 10;
    st_reg_noise_thr.data[3] = (NOISE_PEAK_MIN_THR * 10) >> 8;
    st_reg_noise_thr.data[4] = NOISE_STD_MIN_THR * 10;
    st_reg_noise_thr.data[5] = (NOISE_STD_MIN_THR * 10) >> 8;

    st_reg_chip_noise_thr.data[0] = NOISE_LEN;
    st_reg_chip_noise_thr.data[1] = NOISE_LEN >> 8;
    st_reg_chip_noise_thr.data[2] = NOISE_PEAK_MIN_THR * 10;
    st_reg_chip_noise_thr.data[3] = (NOISE_PEAK_MIN_THR * 10) >> 8;
    st_reg_chip_noise_thr.data[4] = NOISE_STD_MIN_THR * 10;
    st_reg_chip_noise_thr.data[5] = (NOISE_STD_MIN_THR * 10) >> 8;

    for (i = 0; i < PHY_CH_COUNT; i++)
    {
        st_reg_noise_thr.data[6 + i * 4] = (u8)(NOISE_PEAK_MAX_THR * 10);
        st_reg_noise_thr.data[6 + i * 4 + 1] = (u8)((NOISE_PEAK_MAX_THR * 10) >> 8);
        st_reg_noise_thr.data[6 + i * 4 + 2] = (u8)(NOISE_STD_MAX_THR * 10);
        st_reg_noise_thr.data[6 + i * 4 + 3] = (u8)((NOISE_STD_MAX_THR * 10) >> 8);
        st_reg_chip_noise_thr.data[6 + i * 4] = (u8)(NOISE_PEAK_MAX_THR * 10);
        st_reg_chip_noise_thr.data[6 + i * 4 + 1] = (u8)((NOISE_PEAK_MAX_THR * 10) >> 8);
        st_reg_chip_noise_thr.data[6 + i * 4 + 2] = (u8)(NOISE_STD_MAX_THR * 10);
        st_reg_chip_noise_thr.data[6 + i * 4 + 3] = (u8)((NOISE_STD_MAX_THR * 10) >> 8);
        st_reg_offset_thr.data[2 * i] = OFFSET_MAX_THR;
        st_reg_offset_thr.data[2 * i + 1] = OFFSET_MAX_THR >> 8;
    }

    tp_info_init();

    //copy tp info to iic registors
    for (i = 0; i < BU_COUNT; i++)
    {
        st_reg_cus_def_reg2.data[i * 8] = st_tp_info[i].x_min;
        st_reg_cus_def_reg2.data[i * 8 + 1] = st_tp_info[i].x_min >> 8;
        st_reg_cus_def_reg2.data[i * 8 + 2] = st_tp_info[i].y_min;
        st_reg_cus_def_reg2.data[i * 8 + 3] = st_tp_info[i].y_min >> 8;
        st_reg_cus_def_reg2.data[i * 8 + 4] = st_tp_info[i].x_max;
        st_reg_cus_def_reg2.data[i * 8 + 5] = st_tp_info[i].x_max >> 8;
        st_reg_cus_def_reg2.data[i * 8 + 6] = st_tp_info[i].y_max;
        st_reg_cus_def_reg2.data[i * 8 + 7] = st_tp_info[i].y_max >> 8;
        st_reg_cus_def_reg3.data[i] = st_config.motor_mod[i];
    }
}


void cus_flash_para_reinit(void)
{

}

void write_button_thr_2_flash(void)
{
    u8 i;

    for(i = 0; i < BU_COUNT; i++)
    {
        st_config.button_tri_thr[i] = st_algo_para.button_tri_thr[i];
        st_config.button_rel_thr[i] = st_algo_para.button_rel_thr[i];
    }

    st_config.modify_times++;

    st_config.checksum = 0 - calc_checksum_s16((u8*)&st_config, CONFIG_LENG_B - 2);

    hal_write_cfg_2_flash();
}

void write_tp_limit_2_flash(void)
{
    u8 i;

    for(i = 0; i < BU_COUNT; i++)
    {
        st_config.tp_x_min[i] = st_tp_info[i].x_min;
        st_config.tp_x_max[i] = st_tp_info[i].x_max;
        st_config.tp_y_min[i] = st_tp_info[i].y_min;
        st_config.tp_y_max[i] = st_tp_info[i].y_max;
    }

    st_config.modify_times++;

    st_config.checksum = 0 - calc_checksum_s16((u8*)&st_config, CONFIG_LENG_B - 2);

    hal_write_cfg_2_flash();
}

void write_motor_mod_2_flash(void)
{
    st_config.modify_times++;

    st_config.checksum = 0 - calc_checksum_s16((u8*)&st_config, CONFIG_LENG_B - 2);

    hal_write_cfg_2_flash();
}

// s16 dbg_mot[4];

// #define TEST_MOTOR_FOR_V
// #define TEST_SYS_ACK_TIME

void hal_event_detect(void)
{
    u8 i = 0;
    u8 event_val_touch = 0x00;
    static u8 event_pre = 0;
    static u8 int_persist_cnt = 0;

//    //for motor io test
//    if(1)
//    {
//        GPIO_SetMode(motor_io[0], GPIO_Mode_OUTPUT);
//        GPIO_WriteBits(motor_io[0], 0);
//        hal_delay_us(200);
//        GPIO_SetMode(motor_io[0], GPIO_Mode_INPUT);
//        hal_delay_us(200);
//
//        GPIO_SetMode(motor_io[1], GPIO_Mode_OUTPUT);
//        GPIO_WriteBits(motor_io[1], 0);
//        hal_delay_us(200);
//        GPIO_SetMode(motor_io[1], GPIO_Mode_INPUT);
//        hal_delay_us(200);
//    }

/////////////////////////////////debug////////////////////////////////////////
//    #ifdef TEST_SYS_ACK_TIME
//    static u8 cnt = 0;
//    cmd_enter_high_speed_period();

//    if (0 == st_reg_iic_test.data[0])
//    {
//        st_reg_iic_test.data[0] = 10;
//    }

//    if (st_button_det.status[1] == false)
//    {
//        return;
//   }

//    cnt++;

//    if (cnt == 1)
//    {
//        GPIO_SetMode(st_hal_platform.io_buf[1 + GP_IN_COUNT], GPIO_Mode_OUTPUT);
//        GPIO_WriteBits(st_hal_platform.io_buf[1 + GP_IN_COUNT], 0);
//    }
//    else if (cnt == st_reg_iic_test.data[0])
//    {
//        GPIO_SetMode(st_hal_platform.io_buf[1 + GP_IN_COUNT], GPIO_Mode_INPUT);
//    }
//    else if (cnt == st_reg_iic_test.data[0] * 2)
//    {
//        cnt = 0;
//    }

//    return;
//    #endif
/////////////////////////////////////////debug////////////////////////////////////
//    #ifdef TEST_MOTOR_FOR_V
//    static u16 cnt = 0;
//    cmd_enter_high_speed_period();
//    cnt++;

//    if (cnt >= 300)
//    {
//        cnt = 0;

//        if (st_reg_iic_test.data[0] == 1)
//        {
//            GPIO_SetMode(motor_io[0], GPIO_Mode_OUTPUT);
//            GPIO_WriteBits(motor_io[0], 0);
//            hal_delay_us(200);
//            GPIO_SetMode(motor_io[0], GPIO_Mode_INPUT);
//        }
//        else if (st_reg_iic_test.data[0] == 2)
//        {
//            GPIO_SetMode(motor_io[0], GPIO_Mode_OUTPUT);
//            GPIO_WriteBits(motor_io[0], 0);
//            GPIO_SetMode(motor_io[1], GPIO_Mode_OUTPUT);
//            GPIO_WriteBits(motor_io[1], 0);
//            hal_delay_us(200);
//            GPIO_SetMode(motor_io[0], GPIO_Mode_INPUT);
//            GPIO_SetMode(motor_io[1], GPIO_Mode_INPUT);
//        }
//    }

//    return;
//    #endif
////////////////////////////////////////////////////////////////////////////////
    for(i = 0; i < BU_COUNT; i++)
    {
        if(st_button_det.status[i] == true
          )
        {
            event_val_touch |= (1 << i);
        }
    }

    st_reg_edge_event.data[1] = event_val_touch;

    if (st_sys_ctr.io_toggle_cnt > 0)
    {
        if (st_sys_ctr.io_toggle_cnt <= 160)
        {
//            hal_end_toggle_all_io();
        }

        return;
    }


    //not in game mode, only support double touch
    
    if (0 == (st_hal_platform.ap_status & AP_STATUS_GAME_MODE))
    {
        if ((event_val_touch & 3) != 3)
        {
            event_val_touch = 0;
        }
    }

    if (event_val_touch != event_pre)
    {
        st_reg_edge_event.data[0] |= REG_BUSY_KEY;

        for (i = 0; i < GR_COUNT; i++)
        {
            st_time_out.cnt[i] = 0;
        }

        if (st_sys_ctr.dis_report_button_cnt <= 0)
        {
            for(i = 0 ; i < BU_COUNT; i++)
            {
                if (event_val_touch & (1 << i))
                {
//                  GPIO_SetMode(st_hal_platform.io_buf[i + GP_IN_COUNT], GPIO_Mode_OUTPUT);
//                  GPIO_WriteBits(st_hal_platform.io_buf[i + GP_IN_COUNT], 0);
                }
                else
                {
//                  GPIO_SetMode(st_hal_platform.io_buf[i + GP_IN_COUNT], GPIO_Mode_INPUT);
                }

                if (event_val_touch & (1 << i) //down action
                        && ~event_pre & (1 << i)
                        && (st_config.motor_mod[i] - 1) & 1
                        && st_hal_platform.ap_status & AP_STATUS_GAME_MODE //must in game mode, then tri motor
                   )
                {
//                    GPIO_SetMode(motor_io[i], GPIO_Mode_OUTPUT);
//                    GPIO_WriteBits(motor_io[i], 0);
//                    hal_delay_us(200);
//                    GPIO_SetMode(motor_io[i], GPIO_Mode_INPUT);
                    // dbg_mot[2*i]++;
                }
                else if (~event_val_touch & (1 << i)  //up action
                         && event_pre & (1 << i)
                         && (st_config.motor_mod[i] - 1) & 2
                         && st_hal_platform.ap_status & AP_STATUS_GAME_MODE //must in game mode, then tri motor
                        )
                {
//                    GPIO_SetMode(motor_io[i], GPIO_Mode_OUTPUT);
//                    GPIO_WriteBits(motor_io[i], 0);
//                    hal_delay_us(200);
//                    GPIO_SetMode(motor_io[i], GPIO_Mode_INPUT);
                    // dbg_mot[2*i+1]++;
                }
            }
        }
    }

    if (REG_IDLE != st_reg_edge_event.data[0]
            && st_sys_ctr.dis_report_int_cnt <= 0
       )
    {
//        GPIO_SetMode(GPIO_MCU2AP_INT, GPIO_Mode_OUTPUT);
//        GPIO_WriteBits(GPIO_MCU2AP_INT, 0);
//        hal_delay_us(200);
//        GPIO_SetMode(GPIO_MCU2AP_INT, GPIO_Mode_INPUT);

        if (10 == ++int_persist_cnt)
        {
            st_reg_edge_event.data[0] = REG_IDLE;
        }
    }

    if (REG_IDLE == st_reg_edge_event.data[0])
    {
        int_persist_cnt = 0;
    }

    event_pre = event_val_touch;
	
}


void No_game_mode_button_handle(void)
{
    static u16 cnt[BU_COUNT];
    u8 i;

    if (st_hal_platform.ap_status & AP_STATUS_GAME_MODE)
    {
        return;
    }

    for (i = 0; i < BU_COUNT; i++)
    {
        if (st_button_det.status[i])
        {
            cnt[i] += st_sys_ctr.period;

            if (cnt[i] > 1500 && !cnt[1 - i])
            {
                st_button_det.status[i] = 2; //button but not event
            }
        }
        else
        {
            cnt[i] = 0;
        }
    }
}

s16 ev_rel_dbg;
s8 single_force[BU_COUNT];
float qc_coef[BU_COUNT] = {1, 1}; //quick click coef is 0.5 in quick click mode
float slide_coef[BU_COUNT] = {1, 1};

u16 debug_marker_1209[3] = {0};
//extern u16 debug_0324_data;
void button_detect(void)
{
    u8 i;
    u8 max_ch;
    static bool tp_tri_when_ef_set_flag[BU_COUNT];      //to mark wether tp info has been received,when event flag is set
    AdcType force_val_tmp;

    for (i = 0; i < BU_COUNT; i++)
    {
        // max_ch = st_fv.force_rank[st_sensor.ch_start[i]] + st_sensor.ch_start[i];
        // max_ch = st_fv.diff_val[2 * i] > st_fv.diff_val[2 * i + 1] ? 2 * i : 2 * i + 1;

		max_ch = i;
		
        single_force[i] = st_fv.force_val[max_ch];

        if (0 == st_button_det.status[i])
        {
            // if (1 == st_tf.sub_flag[i])
            {
                st_alg_rule.ev_tri[i] |= 0x01;
            }
            
//			if (st_fv.force_val[max_ch] > st_algo_para.button_tri_thr[i] * qc_coef[i])
//          if (st_fv.force_val[max_ch] > st_algo_para.button_tri_thr[i] * qc_coef[i] * (slide_coef[i]<=1?slide_coef[i]:slide_coef[i]*0.5))

			force_val_tmp = st_algo_para.button_tri_thr[i] * qc_coef[i] * (1+0.2*(slide_coef[i]-1));
			
			if(force_val_tmp > 500)
			{
				force_val_tmp = 500;
			}
			//if (st_fv.force_val[max_ch] > st_algo_para.button_tri_thr[i] * qc_coef[i] * slide_coef[i])	
			
			if (st_fv.force_val[max_ch] > force_val_tmp)
            {
                st_alg_rule.ev_tri[i] |= 0x02;
            }

            if (st_fv.force_local[max_ch] > st_algo_para.button_tri_thr[i] * qc_coef[i] * 0.3)	 //v101.9
            {
                st_alg_rule.ev_tri[i] |= 0x04;
            }

            #ifndef NOT_USE_TP //not use tp mode ,test mode, support all event.

            if (st_tp_info[i].state || ~st_hal_platform.ap_status & AP_STATUS_GAME_MODE)  //not in game mode donnot limit tp info
            #endif
            {
                st_alg_rule.ev_tri[i] |= 0x08;
            }

            if (st_fv.force_local[max_ch] < 2 * st_fv.force_val[max_ch]) //to deal with negtive signal rebound problem
            {
                st_alg_rule.ev_tri[i] |= 0x10;
            }

            if ((st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1] > st_raw.sample_buf[max_ch][SAMPLE_COUNT - 2])
                    && (st_raw.sample_buf[max_ch][SAMPLE_COUNT - 2] > st_raw.sample_buf[max_ch][SAMPLE_COUNT - 3])
                    && ((st_raw.sample_buf[max_ch][SAMPLE_COUNT - 3] > st_raw.sample_buf[max_ch][SAMPLE_COUNT - 4])
                        || ~st_hal_platform.ap_status & AP_STATUS_GAME_MODE) // not in game mode , low scan rate ,ignore this
               )
            {
                st_alg_rule.ev_tri[i] |= 0x20;
            }

			if (EM_SM_POWER_OFF!=st_scan_mode_opt.cur_value)	//POWER OFF Disables the report button in power off mode
            {
                st_alg_rule.ev_tri[i] |= 0x40;
            }
			
            if (0x7f == st_alg_rule.ev_tri[i])
            {
                st_button_det.status[i] = true;
                
                if(st_tp_info[i].state)
                {
                    tp_tri_when_ef_set_flag[i] =true;
                }
                else
                {
                    tp_tri_when_ef_set_flag[i] =false;
                }
            }
        }
        else
        {
            static u8 tp_release_cnt[BU_COUNT] = {0};

            if(st_tp_info[i].state)
            {
                tp_release_cnt[i] = 0;
            }
            else
            {
                tp_release_cnt[i] ++;
            }

            debug_marker_1209[0] = 17;

            // if (0 == st_tf.sub_flag[i])								 {st_alg_rule.ev_rel[i] |= 0x01;}
            if (st_fv.force_val[max_ch] < st_algo_para.button_rel_thr[i]  * qc_coef[i] / (1.5*slide_coef[i]))  //slide mode hard to rel
            {
                st_alg_rule.ev_rel[i] |= 0x02;
            }

//            #if 1           //fw ver 08

//            if(tp_release_cnt[i] > 3)
//            {
//                st_alg_rule.ev_rel[i] |= 0x04;
//            }

//            #else           //fw ver 09
//            else    //walt1209
//            {
//                if(tp_release_cnt[i] > 3)
//                {
//                    reset_base_from_cur_raw(i);
//                }
//            }

//            #endif //

            if((tp_release_cnt[i] > 3)
                &&(tp_tri_when_ef_set_flag[i])
                &&(st_hal_platform.ap_status & AP_STATUS_GAME_MODE)
            )
            {
                st_alg_rule.ev_rel[i] |= 0x04;
            }

            if (st_alg_rule.ev_rel[i])
            {
                st_button_det.status[i] = false;
                ev_rel_dbg |= 1 << (8 * i);

                debug_marker_1209[0] = 1;
                
//                debug_0324_data |= 1<<0;
            }
        }
    }

    No_game_mode_button_handle();
}

void calc_temp_coef(float temp_val)
{

#define C1 (TCC_Table[i - 1].coeff[m])
#define C2 (TCC_Table[i].coeff[m])
#define T1 (TCC_Table[i - 1].temp)
#define T2 (TCC_Table[i].temp)
#define T0 (temp_val)

    u8 m, i;

    for(i = 0; i < TCC_TABLE_MAX; i++)
    {
        if(temp_val < TCC_Table[i].temp)
        {
            if(i == 0)
            {
                for(m = 0; m < PHY_CH_COUNT; m++)
                {
                    st_common.temp_coef_buf[m] = TCC_Table[i].coeff[m];
                }
            }
            else
            {
                for(m = 0; m < PHY_CH_COUNT; m++)
                {
                    st_common.temp_coef_buf[m] = C1 * C2 * (T2 - T1) / (C1 * (T0 - T1) + C2 * (T2 - T0));
                }
            }

            break;
        }
    }

    if(i >= TCC_TABLE_MAX)
    {
        for(m = 0; m < PHY_CH_COUNT; m++)
        {
            st_common.temp_coef_buf[m] = TCC_Table[TCC_TABLE_MAX - 1].coeff[m];
        }
    }
//	printf("coef:%f, %f\n\n",  st_common.temp_coef_buf[0], st_common.temp_coef_buf[1]);

    for(i = 0; i < PHY_CH_COUNT; i++)
    {
        st_reg_temp_info.data[4 + 2 * i] = (u16)(st_common.temp_coef_buf[i] * CHANNELCAL_DATA_GAINFACTOR);
        st_reg_temp_info.data[4 + 2 * i + 1] = (u16)(st_common.temp_coef_buf[i] * CHANNELCAL_DATA_GAINFACTOR) >> 8;
    }

#undef C1
#undef C2
#undef T1
#undef T2	
#undef T0
}

void give_tp_temp_info_for_simulation(void)
{
    static u8 test_counter = 0;

    test_counter ++;

    //
    if(test_counter > 192)
    {
        st_tp_info[0].x_point = 980;
        st_tp_info[0].y_point = 200;
        st_tp_info[0].state = false;

        st_tp_info[1].x_point = 100;
        st_tp_info[1].y_point = 2200;
        st_tp_info[1].state = true;
    }

    else if(test_counter > 128)
    {
        st_tp_info[0].x_point = 980;
        st_tp_info[0].y_point = 200;
        st_tp_info[0].state = false;

        st_tp_info[1].x_point = 100;
        st_tp_info[1].y_point = 2200;
        st_tp_info[1].state = false;
    }

    else if(test_counter > 64)
    {
        st_tp_info[0].x_point = 980;
        st_tp_info[0].y_point = 200;
        st_tp_info[0].state = true;

        st_tp_info[1].x_point = 100;
        st_tp_info[1].y_point = 2200;
        st_tp_info[1].state = false;
    }

    else
    {
        st_tp_info[0].x_point = 980;
        st_tp_info[0].y_point = 200;
        st_tp_info[0].state = true;

        st_tp_info[1].x_point = 100;
        st_tp_info[1].y_point = 2200;
        st_tp_info[1].state = true;
    }
}

#ifdef TCC_TABLE_UPDATE_FUNCTION_SUPPORT  
static s16 interp_quadratic_for_tcc_table(u16 coordinate_x, u16 coordinate_y, u8 sensor_id,u16 tab_indx)
{
#define X_INTERVAL (10)
#define Y_INTERVAL (10)

//    u8 tp_id = sensor_id / 2;

	u8 tp_id = sensor_id;

    u16 coor_x = coordinate_x * X_INTERVAL / st_tp_info[tp_id].x_delta;
    u16 coor_y = coordinate_y * Y_INTERVAL / st_tp_info[tp_id].y_delta;
    u16 coor_x0 = st_tp_info[tp_id].x_start * X_INTERVAL / st_tp_info[tp_id].x_delta;
    u16 coor_y0 = st_tp_info[tp_id].y_start * Y_INTERVAL / st_tp_info[tp_id].y_delta;

    // 计算插值区间的index起点
    s16 i = (coor_x - coor_x0) / X_INTERVAL;
    s16 j = (coor_y - coor_y0) / Y_INTERVAL;

    i = MAX(0, i);
    j = MAX(0, j);
    i = MIN(i, st_tp_info[tp_id].x_count - 2);
    j = MIN(j, st_tp_info[tp_id].y_count - 2);

    // 坐标按起点归0�?
    s16 cx = coor_x - i * X_INTERVAL - coor_x0;
    s16 cy = coor_y - j * Y_INTERVAL - coor_y0;

    // 超过范围则按边缘值计�?
    cx = MAX(0, cx);
    cy = MAX(0, cy);
    cx = MIN(cx, X_INTERVAL);
    cy = MIN(cy, Y_INTERVAL);

    s32 fit_val = 0;
    s32 fit_val_x;
	
//	printf("sensor_id: %d\n", sensor_id);
//	printf("tab:%d\n", tab_indx);
//	printf("coor: %d,%d\n", i, j);
//	printf("cxy: %d,%d\n", cx, cy);

    for (u8 yk = 0; yk < 2; yk++)
    {
        u8 yk1 = (yk + 1) % 2;

        s16 mo1 = 100* temperature_model[tab_indx].p_model[sensor_id][i + 0 + (j + yk) * st_tp_info[tp_id].x_count];
        s16 mo2 = 100* temperature_model[tab_indx].p_model[sensor_id][i + 1 + (j + yk) * st_tp_info[tp_id].x_count];
		
//		printf("mo:%d, %d\n", mo1, mo2);

        // mo1 = MIN(MAX_MODEL_LIMIT, mo1); //模板限幅
        // mo2 = MIN(MAX_MODEL_LIMIT, mo2);

		//prevent divisor by zero  walt0310+
        {
            if ((mo1 >= - 1) && (mo1<= 1))
            {
                mo1 =1;
            }

            if ((mo2 >= - 1) && (mo2 <= 1))
            {
                mo2 =1;
            }
        }
		
        mo1 = 100000 / mo1; //转换�?00g对应的信号量
        mo2 = 100000 / mo2; //转换�?00g对应的信号量

        fit_val_x = mo1 * (X_INTERVAL - cx) \
                    + mo2 * (cx);

        fit_val += fit_val_x * (cy - yk1 * Y_INTERVAL) / (yk - yk1); // y方向插�?
        
		
    }
	

    if (0 == fit_val)
    {
        return MAX_MODEL_LIMIT;
    }

    fit_val = 10000000 / fit_val; //因为X_INTERVAL  X_INTERVAL都是10 所以这里直接简化了

    fit_val = MIN(fit_val, MAX_MODEL_LIMIT); //限幅
    fit_val = MAX(fit_val, -MAX_MODEL_LIMIT); //限幅
//	printf("val:%d\n\n", fit_val);

    return fit_val;
}
#endif //TCC_TABLE_UPDATE_FUNCTION_SUPPORT   

//
void update_tcc_table_accord_tp_info(void)
{
#ifdef TCC_TABLE_UPDATE_FUNCTION_SUPPORT    
    u16 i=0, j=0, k=0;
    u16 tp_index=0;
    u16 tab_indx=0;
    
    //    u16 tab_indx, sensor_num, model_num;

    s16 tmep_coef;

//    static s16 tmp_coef_buf[MAX_SENSOR_COUNT][MAX_MODELCOUNT];

    for(tab_indx = 0; tab_indx < TCC_TABLE_MAX; tab_indx++)
    {
        //copy data to buffer,to use same interp_quadratic() function
        //temperature_coef has been multiply by 10,so need *100 again---->at last multiply by 1000
//        for(sensor_num = 0; sensor_num < MAX_SENSOR_COUNT; sensor_num++)
//        {
//            for(model_num = 0; model_num < MAX_MODELCOUNT; model_num++)
//            {
//                tmp_coef_buf[sensor_num][model_num] = 100 * (temperature_model[tab_indx].p_model[sensor_num][model_num]);
//            }
//        }

        for(i = 0; i < CH_COUNT; i++)
        {
//            for(j = 0; j < BU_COUNT; j++)
//            {
//                for(k = 0; k < MAX_NUM_OF_LOGIC_CHANNEL_FOR_A_BUTTON; k++)
//                {
//                    if((pt154_chnl_to_button_map.channel_to_button_map[j][k]) == i)
//                    {
//                        tp_index = j;
//						printf(">>>>>>>%d\n", tp_index);
//                    }
//                }
//            }

            //use default coef if no tp press
            if (!st_tp_info[i].state)
            {
                TCC_Table[tab_indx].coeff[i] = TCC_Table_Default[tab_indx].coeff[i];
            }
            else
            {
                tmep_coef = interp_quadratic_for_tcc_table(st_tp_info[i].x_point, st_tp_info[i].y_point, i,tab_indx);
                TCC_Table[tab_indx].coeff[i] = (float)tmep_coef / 1000;
            }

        }
    }
#endif //TCC_TABLE_UPDATE_FUNCTION_SUPPORT    
}

void update_reg_data_to_chip_temperature(void)
{
  	st_common.temprature_cur = st_common.temprature_cur_aw;
	st_common.temprature_real = st_common.temprature_cur - st_common.temprature_init + st_common.temprature_init_bat;
	st_reg_temp_info.data[0] = (s8)(st_common.temprature_real * 10);
	st_reg_temp_info.data[1] = (s16)(st_common.temprature_real * 10) >> 8;
}

void temprature_monitor(void)
{
    static u16 tm_cnt = TEMPRATURE_MONITOR_INTERVAL;
    float temp_val=0;

    tm_cnt += st_sys_ctr.period;

    if(tm_cnt > 200  + st_sys_ctr.period)
    {
        tm_cnt = 0;

//      temp_val = hal_temperature_read();

		temp_val = st_common.temprature_cur_aw;
		
        st_common.temprature_cur = st_common.temprature_cur * 0.9 + temp_val * 0.1;
        
//		0.0 = x - 22 + 244
        st_common.temprature_real = st_common.temprature_cur - st_common.temprature_init + st_common.temprature_init_bat;
		
        if (st_common.temprature_real >= 40)
        {
            st_fv.err_state[0] |= FORCE_ERR_STATE_HIGH_TEMP;
        }
        else
        {
            st_fv.err_state[0] &= ~FORCE_ERR_STATE_HIGH_TEMP;
        }

        if (st_common.temprature_real >= 52)
        {
            st_fv.err_state[0] |= FORCE_ERR_STATE_EXTREMELY_HIGH_TEMP;
        }
        else
        {
            st_fv.err_state[0] &= ~FORCE_ERR_STATE_EXTREMELY_HIGH_TEMP;
        }

        if (st_common.temprature_real <= -10)
        {
            st_fv.err_state[0] |= FORCE_ERR_STATE_LOW_TEMP;
        }
        else
        {
            st_fv.err_state[0] &= ~FORCE_ERR_STATE_LOW_TEMP;
        }

        update_tcc_table_accord_tp_info();

        calc_temp_coef(st_common.temprature_real);

        st_reg_temp_info.data[0] = (s8)(st_common.temprature_real * 10);
        st_reg_temp_info.data[1] = (s16)(st_common.temprature_real * 10) >> 8;
    }
}

static s16 fv_local[CH_COUNT];
static s16 fv_buf[CH_COUNT][SAMPLE_COUNT];
static s16 fv_peak[CH_COUNT];

s16 _calc_local_fv(u8 index)
{
#define MIN_VALID_SLP_THR  7

    u8 i;
    u8 update_num = 0;
    s16 local_sig = 0;
    s16 diff_temp;

    for (i = 1; i < LOCAL_BUFF_LENG; i++)
    {
        diff_temp = fv_buf[index][SAMPLE_COUNT - 1] - fv_buf[index][SAMPLE_COUNT - i];

        if (0 == local_sig)
        {
            if ((diff_temp > MIN_VALID_SLP_THR
                    && fv_buf[index][SAMPLE_COUNT - 1] - fv_buf[index][SAMPLE_COUNT - 1 - i - 1] > MIN_VALID_SLP_THR
                    && fv_buf[index][SAMPLE_COUNT - 1] - fv_buf[index][SAMPLE_COUNT - 1 - i - 2] > MIN_VALID_SLP_THR)
                    || (diff_temp < -MIN_VALID_SLP_THR
                        && fv_buf[index][SAMPLE_COUNT - 1] - fv_buf[index][SAMPLE_COUNT - 1 - i - 1] < -MIN_VALID_SLP_THR
                        && fv_buf[index][SAMPLE_COUNT - 1] - fv_buf[index][SAMPLE_COUNT - 1 - i - 2] < -MIN_VALID_SLP_THR)
               )
            {
                local_sig = diff_temp;
            }

            continue;
        }

        if ((local_sig < diff_temp && local_sig > 0)
                || (local_sig > diff_temp && local_sig < 0)
           )
        {
            local_sig = diff_temp;
            update_num = 0;
        }
        else
        {
            update_num++;
        }

        if (update_num > 4)
        {
            break;
        }
    }

    return (local_sig);
}

void fv_dispose(void)
{
    u8 i;

    for (i = 0; i < CH_COUNT; i++)
    {
        memcpy(&(fv_buf[i][0]), &(fv_buf[i][1]), (SAMPLE_COUNT - 1) * sizeof(AdcType));
        fv_buf[i][SAMPLE_COUNT - 1] = st_fv.force_val[i];

        fv_local[i] = _calc_local_fv(i);
    }
}


//cmd:0 reset state
void quick_click_rel_judge(u8 group, u8 cmd)
{
    static u8 state[GR_COUNT];
    static u16 cnt[GR_COUNT];
    u8 max_ch;

    debug_marker_1209[1] = 0;

    //max_ch = st_fv.diff_val[2 * group] > st_fv.diff_val[2 * group + 1] ? 2 * group : 2 * group + 1;

	max_ch = group;

    if (0 == cmd)
    {
        debug_marker_1209[1] = 11;

        state[group] = 0;
        return;
    }

    //slide state donnot use this function
    if (slide_coef[group] != 1)
    {
        state[group] = 0;

        debug_marker_1209[1] = 12;
    }

    if (0 == state[group])
    {
        if (fv_local[max_ch] < -st_algo_para.button_tri_thr[group]
                && st_fv.force_local[max_ch] < -st_algo_para.button_tri_thr[group]
           )
        {
            state[group] = 1;

            debug_marker_1209[1] = 13;
        }
    }
    else if (1 == state[group])
    {
        debug_marker_1209[1] = 16;

        if (fv_local[max_ch] > st_algo_para.button_tri_thr[group]
                && st_fv.force_local[max_ch] > st_algo_para.button_tri_thr[group]
           )
        {
            state[group] = 2;
            cnt[group] = 0;

            debug_marker_1209[1] = 14;
        }
    }
    else if (2 == state[group])
    {

        debug_marker_1209[1] = 15;

        cnt[group]++;

        if (cnt[group] > 20)
        {
            state[group] = 1;
        }

        if (fv_local[max_ch] < -st_algo_para.button_tri_thr[group])
        {
            st_button_det.status[group] = 0;
            ev_rel_dbg |= 64 << (8 * group);
            state[group] = 0;

            debug_marker_1209[1] = 2;
        }
    }
}

#define TP_DISTANCE_HISTORY_LEN     100
#define EXTRM_FAST_MOVE_THR         75
typedef struct{
    u16 distance_history[TP_DISTANCE_HISTORY_LEN];
//    s16 tp_x_last_frame;
//    s16 tp_y_last_frame;
    bool tp_history_valid_flag;
    u16 data_pointer;
} ST_TP_HISTORY_INFO;

ST_TP_HISTORY_INFO tp_history_info[GR_COUNT];

bool tp_is_extremely_fast_moved(u8 group)
{
    if(!(tp_history_info[group].tp_history_valid_flag))
    {
        return false;
    }
    
    for(u16 i=0;i<tp_history_info[group].data_pointer;i++)
    {
        if(tp_history_info[group].distance_history[i] >EXTRM_FAST_MOVE_THR)
        {
            return true;
        }
    }
    
    return false;
}

u16 ef_rel_cnt[GR_COUNT] ={0};
//event flag dispose
void ef_dispose(void)
{
    u8 i;
    static u16 tri_cnt[GR_COUNT];
    static u16 rel_cnt[GR_COUNT];
    static u16 unmove_cnt[GR_COUNT];
    static s16 sig_up_max[CH_COUNT];
    static s16 tp_tri_x[GR_COUNT];
    static s16 tp_tri_y[GR_COUNT];
    s16 tp_distance[GR_COUNT];
    u8 max_ch, min_ch;

    fv_dispose();

    ev_rel_dbg = 0;
    
    debug_marker_1209[2] =8;

    //each group, separate dispose
    for (i = 0; i < GR_COUNT; i++)
    {
        if (st_button_det.status[i])
        {
            if (0 == tri_cnt[i] && rel_cnt[i] < 30)
            {
                qc_coef[i] = 0.5;
            }

            if (tri_cnt[i] > 100)
            {
                qc_coef[i] = 1;
            }

            tri_cnt[i]++;
            rel_cnt[i] = 0;

            //fv_peak[2 * i] = st_fv.force_peak[2 * i];
            //fv_peak[2 * i + 1] = st_fv.force_peak[2 * i + 1];

			fv_peak[i] = st_fv.force_peak[i];
            
            tp_history_info[i].tp_history_valid_flag =true;
        }
        else
        {
            if (rel_cnt[i] > 50)
            {
                qc_coef[i] = 1;
            }

            tri_cnt[i] = 0;
            rel_cnt[i]++;

            //fv_peak[2 * i] = 0;
            //fv_peak[2 * i + 1] = 0;

			fv_peak[i] = 0;
			
            tp_history_info[i].tp_history_valid_flag =false;
        }

        //max_ch = st_fv.diff_val[2 * i] > st_fv.diff_val[2 * i + 1] ? 2 * i : 2 * i + 1;
        //min_ch = 4 * i + 1 - max_ch;

		max_ch = i;
		min_ch = i;
		
        ////timeout related to event, instead of forceflag
        if (1 == tri_cnt[i])
        {
            st_time_out.cnt[i] = 0;

            //sig_up_max[2 * i] = 0;
            //sig_up_max[2 * i + 1] = 0;

			sig_up_max[i] = 0;

            tp_tri_x[i] = st_tp_info[i].x_point;
            tp_tri_y[i] = st_tp_info[i].y_point;

            quick_click_rel_judge(i, 0); // init state
            
            tp_history_info[i].data_pointer =0;
        }

        if(tp_history_info[i].tp_history_valid_flag)
        {   
            tp_history_info[i].distance_history[tp_history_info[i].data_pointer] =ABSDATA(tp_tri_x[i], st_tp_info[i].x_point) + ABSDATA(tp_tri_y[i], st_tp_info[i].y_point);
            tp_history_info[i].data_pointer +=1;
            
            if(TP_DISTANCE_HISTORY_LEN  ==(tp_history_info[i].data_pointer))
            {
               tp_history_info[i].data_pointer =0; 
            }
            
//            tp_history_info[i].tp_x_last_frame = st_tp_info[i].x_point;
//            tp_history_info[i].tp_y_last_frame = st_tp_info[i].y_point;
        }
            
        tp_distance[i] = ABSDATA(tp_tri_x[i], st_tp_info[i].x_point) + ABSDATA(tp_tri_y[i], st_tp_info[i].y_point);

        //sig_up_max[2 * i] = MAX(sig_up_max[2 * i], fv_local[2 * i]);
        //sig_up_max[2 * i + 1] = MAX(sig_up_max[2 * i + 1], fv_local[2 * i + 1]);

		sig_up_max[i] = MAX(sig_up_max[i], fv_local[i]);

        if (4 == slide_coef[i])
        {
            unmove_cnt[i] = 20;
        }

        if (unmove_cnt[i] > 0)
        {
            unmove_cnt[i]--;
        }

        //special event release logic, only valid not in slide mode
        if (st_button_det.status[i]
                && (unmove_cnt[i] == 0|| qc_coef[i] < 1)
                && tp_distance[i] < 150
				&&(!(tp_is_extremely_fast_moved(i)))	
           )
        {
            
//            debug_0324_data |= 1<<1;
            
            debug_marker_1209[2] =9;
            
            //local down rel

            if  ( st_fv.force_val[max_ch] < 0.7 * (st_fv.force_val[max_ch] - fv_local[max_ch])
                    && fv_local[max_ch] < -FF_LOCAL_TH
                    && calc_min_slp_prd(max_ch, 3) < -6
                    && tri_cnt[i] < 200
                    && fv_local[min_ch] < 0
                )
            {
                st_button_det.status[i] = 0;
                ev_rel_dbg |= 2 << (8 * i);
                
                debug_marker_1209[2] =7;
            }

            //local up & down proportion2
/*
            if (fv_local[max_ch] + sig_up_max[max_ch] * 0.6 < 0 && fv_local[min_ch] < 0)
            {
                st_button_det.status[i] = 0;
                ev_rel_dbg |= 4 << (8 * i);
                
                debug_marker_1209[2] =6;
            }
*/

            //local up & down proportion2
/*          
			if (fv_local[max_ch] + sig_up_max[max_ch] * 0.4 < 0 && fv_local[min_ch] + sig_up_max[min_ch] * 0.5 < 0)
            {
                st_button_det.status[i] = 0;
                ev_rel_dbg |= 8 << (8 * i);
                
                debug_marker_1209[2] =5;
            }
*/
/*
            //cmp with peak val
            if (st_fv.force_val[max_ch] < 0.3 * fv_peak[max_ch])
            {
                st_button_det.status[i] = 0;
                ev_rel_dbg |= 16 << (8 * i);
                
                debug_marker_1209[2] =4;
            }
*/

        }

        if (st_button_det.status[i])
        {
//            debug_marker_1209[2] =10;
            
//072-            if (st_fv.force_val[max_ch] < 0.2 * fv_peak[max_ch])
//072-            {
//                st_button_det.status[i] = 0;
//                ev_rel_dbg |= 32 << (8 * i);
//                
//                debug_marker_1209[2] =3;
//072-             }

            if (tp_distance[i] >= 150 && tri_cnt[i] > 50)
            {
                quick_click_rel_judge(i, 1); //normal jduge process
            }
        }
        
        ef_rel_cnt[i] =rel_cnt[i];
    }   
}

s16 dbg_sum_recd[2];
#define EXTREMELY_MOVE (2)
#define MIDDLE_MOVE (1)
#define NO_MOVE (0)
static u8 move_state[BU_COUNT];
#define TP_BUF_LEN 7
s16 tp_buf_x[BU_COUNT][TP_BUF_LEN];
s16 tp_buf_y[BU_COUNT][TP_BUF_LEN];

#define LEVEL0 10
#define LEVEL1 50
#define LEVEL2 100

void set_slide_coef_from_tpbuf(u8 group)
{
    u8 i;
    s16 sum = 0, tmp_x = 0, tmp_y = 0;
	static s16 sum_pre[2][100];
	s16 sum_pre_max=0;

    for (i = 0; i < TP_BUF_LEN - 1; i++)
    {
        tmp_x = ABSDATA(tp_buf_x[group][i], tp_buf_x[group][i + 1]);

        tmp_y = ABSDATA(tp_buf_y[group][i], tp_buf_y[group][i + 1]);

        if (sqrt(tmp_x*tmp_x+tmp_y*tmp_y) > 500)
        {
            tmp_y = 0;
            tmp_x = 0;
        }

        sum += sqrt(tmp_x*tmp_x+tmp_y*tmp_y);
    }

    dbg_sum_recd[group] = sum;

	for(i=0;i<99;i++){
		sum_pre[group][i+1]=sum_pre[group][i];
	}
	sum_pre[group][0]=sum;

	for(i=0;i<99;i++){
		if(sum_pre_max<sum_pre[group][i]){
			sum_pre_max=sum_pre[group][i];
		}
	}

	//printf("tpsum[%d]\t%d\n", group, dbg_sum_recd[group]);
	//printf("sum_pre_max\t%d\n", sum_pre_max);

    if (EXTREMELY_MOVE & move_state[group])
    {
        if (sum_pre_max < 2)
        {
            move_state[group] = NO_MOVE;
            slide_coef[group] = 1;
        }
        else if (sum < LEVEL1)
        {
     //       slide_coef[group] = 2;
        }
        else if (sum < LEVEL2)
        {
            slide_coef[group] = 3;
        }
        else
        {
            slide_coef[group] = 4;
        }
    }
    else if (MIDDLE_MOVE & move_state[group])
    {
        if (sum_pre_max < 2)
        {
            move_state[group] = NO_MOVE;
            slide_coef[group] = 1;
        }
        else if (sum < LEVEL1)
        {
            slide_coef[group] = 2;
        }
        else
        {
            slide_coef[group] = 4;
            move_state[group] = EXTREMELY_MOVE;
        }
    }
    else if (NO_MOVE == move_state[group])
    {
        if (sum > LEVEL2)
        {
            move_state[group] = EXTREMELY_MOVE;
            slide_coef[group] = 4;
        }
        else if (sum > LEVEL1)
        {
            move_state[group] = MIDDLE_MOVE;
            slide_coef[group] = 2;
        }
        else
        {
            slide_coef[group] = 1;
        }
    }

    //in idle mode donnot use slidecoef
    if (st_scan_mode_opt.cur_value > EM_SM_HIGH_SPEED)
    {
        slide_coef[group] = 1;
    }
}

void monitor_tp_reg(void)
{
    int i;

    //checksum pass, get tp val
//    if (0 == calc_checksum_s16(st_reg_cus_def_reg4.data, sizeof(st_reg_cus_def_reg4.data))
//       )
//    {
//        for (i = 0; i < MAX_TP_COUNT; i++)
//        {
//            st_tp_info[i].x_point = (s16)(st_reg_cus_def_reg4.data[i * 4] + ((u16)st_reg_cus_def_reg4.data[i * 4 + 1] << 8));
//            st_tp_info[i].y_point = (s16)(st_reg_cus_def_reg4.data[i * 4 + 2] + ((u16)st_reg_cus_def_reg4.data[i * 4 + 3] << 8));
//        }
//    }
	
    for (i = 0; i < MAX_TP_COUNT; i++)
    {

        memcpy(&tp_buf_x[i][0], &tp_buf_x[i][1], sizeof(s16) * (TP_BUF_LEN - 1));
        memcpy(&tp_buf_y[i][0], &tp_buf_y[i][1], sizeof(s16) * (TP_BUF_LEN - 1));
		
        tp_buf_x[i][TP_BUF_LEN - 1] = st_tp_info[i].x_point;
        tp_buf_y[i][TP_BUF_LEN - 1] = st_tp_info[i].y_point;

        set_slide_coef_from_tpbuf(i);

        if (st_tp_info[i].x_point > 0 && st_tp_info[i].y_point > 0)
        {
            st_tp_info[i].state = 1;
        }
        else
        {
            st_tp_info[i].state = 0;
        }

        if (st_tp_info[i].state)
        {
            st_tp_info[i].tri_cnt++;
            st_tp_info[i].rel_cnt = 0;
        }
        else
        {
            st_tp_info[i].tri_cnt = 0;
            st_tp_info[i].rel_cnt++;
        }

        // //new tp touch, reset baseline
        // if (st_tp_info[i].state && !tp_state_pre
        // 	&& st_tp_info[i].rel_cnt >= 3)
        // {
        // 	reset_base_from_valley_raw(i, 10000, 20);
        // }

        if (st_hal_platform.ap_status & AP_STATUS_GAME_MODE
                && !st_tp_info[i].state
                && st_ff.sub_flag[i] == em_negtive_flag
                && st_ff.tri_cnt[i] > 30
           )
        {
            reset_base_from_cur_raw(i);
        }
    }
}

void cus_monitor(void)
{
    u8 i;

    monitor_tp_reg();
    ef_dispose();

    if (st_hal_platform.ap_status & AP_STATUS_GAME_MODE) //game mode
    {
        st_scan_mode_opt.enter_low_speed_cnt = 0;
    }

    temprature_monitor();

    for (i = 0; i < GP_COUNT; i++)
    {
//        st_reg_gpio_status.data[i * 2 + 1] = (st_reg_gpio_status.data[i * 2 + 1] & 0X0C) | hal_get_io_reg_status(st_hal_platform.io_buf[i]);
    }
}

void cus_cmd_detect(uint8_t cmd)
{
    switch(cmd)
    {
        case CMD_GPIO_STATUS:
            break;

        case CMD_IO_AUTO_TEST:
//            hal_start_toggle_all_io();
            st_sys_ctr.io_toggle_cnt = 230;
            st_scan_mode_opt.enter_low_speed_cnt = 0;
            break;

        default:
            break;
    }
}

void cus_handshake_detect(u8 handshake)
{
    u8 i;

    switch (handshake)
    {
        case REG_AP_STATUS:
            if (st_hal_platform.ap_status & AP_STATUS_GAME_MODE) //game mode
            {
                cmd_enter_high_speed_period();
                st_scan_mode_opt.cur_value = EM_SM_HIGH_SPEED;
                st_adc.avg_count = AVG_POWER_COUNT;

                for (int i = 0; i < PHY_CH_COUNT; i++)
                {
                    for (int j = 0; j < AVG_POWER_COUNT; j++)
                    {
                        st_adc.avg_buf[i][j] = st_common.adc_data[i];
                    }
                }
            }
            else
            {
                st_scan_mode_opt.cur_value = EM_SM_LOW_SPEED;
                st_adc.avg_count = AVG_POWER_COUNT_IDLE;
            }

            break;

        case REG_CUS_DEFINE1:

            break;

        case REG_CUS_DEFINE2:

            break;

        case REG_CUS_DEFINE3:

            for (i = 0; i < MAX_TP_COUNT; i++)
            {
                st_config.motor_mod[i] = st_reg_cus_def_reg3.data[i];
            }

            // write_motor_mod_2_flash();

            break;

        case REG_CUS_DEFINE4:

            for (i = 0; i < MAX_TP_COUNT; i++)
            {
                st_tp_info[i].x_point = (s16)(st_reg_cus_def_reg4.data[i * 4] + ((u16)st_reg_cus_def_reg4.data[i * 4 + 1] << 8));
                st_tp_info[i].y_point = (s16)(st_reg_cus_def_reg4.data[i * 4 + 2] + ((u16)st_reg_cus_def_reg4.data[i * 4 + 3] << 8));
            }

            break;

        default:
            break;
    }

}

#ifdef EN_OUTP_SINGLE_FORCE
void output_single_force(u8 mode_select)
{
    // u8 i = 0;
    s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
    u16 len;
    // u32 overflow_cnt;

    if(0 == st_interaction[mode_select].rw_cound_buf[0])
    {
        // st_sys_ctr.dis_report_button_cnt = 1000; //1s dis button report
        // st_sys_ctr.dis_report_int_cnt = 1000;    //1s dis int report
        st_scan_mode_opt.enter_low_speed_cnt = 0;

        *lpdata++ = single_force[0];//0
        *lpdata++ = single_force[1];//1

        len = lpdata - (s16 *)&st_interaction[mode_select].rw_data_buf[0];
        *lpdata = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, len << 1);
        st_interaction[mode_select].rw_cound_buf[0] = ((len << 1) + 2);

// /////////////////////用于做数据同�?/////////////////////////////////
// 		overflow_cnt = 200000;    //about 100ms
// 		while(st_interaction[mode_select].rw_cound_buf[0] && overflow_cnt--);
// /////////////////////用于做数据同�?/////////////////////////////////

    }
}

#endif


#define DEBUG_F5_FUNCTION_SELECT  2

#if (1==DEBUG_F5_FUNCTION_SELECT)
void output_user_debug_data(u8 mode_select)
{
    u8 i = 0;
    s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
    u16 len;
    u32 overflow_cnt;

    if(0 == st_interaction[mode_select].rw_cound_buf[0])
    {
        // st_sys_ctr.dis_report_button_cnt = 1000; //1s dis button report
        st_sys_ctr.dis_report_int_cnt = 1000;    //1s dis int report
        st_scan_mode_opt.enter_low_speed_cnt = 0;

        *lpdata++ = st_reg_edge_event.data[1];//0

         static s16 cnt;
        *lpdata++ = cnt++;
        
        for(i = 0; i < CH_COUNT; i++)
        {
            *lpdata++ = st_raw.current_buf[i];
        }
        
        for(i = 0; i < CH_COUNT; i++)
        {
            *lpdata++ = st_base.base[i];
        }
        
        for(i = 0; i < CH_COUNT; i++)
        {
            *lpdata++ = st_fv.force_val[i];
        }
        
        for(i = 0; i < CH_COUNT; i++)
        {
            *lpdata++ = st_common.balance_buf[i] * 1000;
        }
        
        for(i = 0; i < MAX_TP_COUNT; i++)
        {
            *lpdata++ = st_tp_info[i].x_point;
            *lpdata++ = st_tp_info[i].y_point;
        }
        
        for(i = 0; i < GR_COUNT; i++)
        {
            *lpdata++ = st_alg_rule.ev_tri[i];
        }

        for(i = 0; i < GR_COUNT; i++)
        {
            *lpdata++ = st_alg_rule.ev_rel[i];
        }
        
        *lpdata++ = ev_rel_dbg;
        
        for(i = 0; i < BU_COUNT; i++)
        {
            *lpdata++ = qc_coef[i] * 1000;
        }

        for(i = 0; i < BU_COUNT; i++)
        {
            *lpdata++ = slide_coef[i] * 1000;
        }
        
        for(i = 0; i < CH_COUNT; i++)
        {
            *lpdata++ = st_fv.force_local[i];
        }
        
//        extern uint32_t algorithm_counter;
//        *lpdata++ =algorithm_counter>>16;
//        *lpdata++ =(uint16_t)algorithm_counter;
        
        len = lpdata - (s16 *)&st_interaction[mode_select].rw_data_buf[0];
        *lpdata = 0 - calc_checksum_s16((u8 *)st_interaction[mode_select].rw_data_buf, len << 1);
        st_interaction[mode_select].rw_cound_buf[0] = ((len << 1) + 2);

/////////////////////?????????????//////////////////////////////////
        overflow_cnt = 200000;    //about 100ms

        while(st_interaction[mode_select].rw_cound_buf[0] && overflow_cnt--);

/////////////////////?????????????//////////////////////////////////

    }

}
#else
unsigned char output_user_debug_data(void* log)
{
    u8 i = 0;
    s16 *lpdata = (s16*)log;
    u16 len;
    u32 overflow_cnt;

    // st_sys_ctr.dis_report_button_cnt = 1000; //1s dis button report
    st_sys_ctr.dis_report_int_cnt = 1000;    //1s dis int report
    st_scan_mode_opt.enter_low_speed_cnt = 0;

    *lpdata++ = st_reg_edge_event.data[1];//0
    *lpdata++ = st_ff.all_flag;//1
    *lpdata++ = st_tf.all_flag;//2

    *lpdata++ = st_reg_fw_ver.data[3] | ((s16)st_reg_fw_ver.data[2] << 8); //3
    *lpdata++ = st_hal_platform.ch_flag;//4
    *lpdata++ = st_dac.total_cali_times; //5
    *lpdata++ = st_hal_platform.ap_status; //6
    *lpdata++ = st_reg_gpio_status.data[1] | ((s16)st_reg_gpio_status.data[3] << 8); //need to confrim 	7

     static s16 cnt;
    *lpdata++ = cnt++;	//8
    
    *lpdata++ = st_fv.err_state[0] | (st_fv.err_state[1] << 8);	//9
    
    for(i = 0; i < BU_COUNT; i++) //12-13
    {
        *lpdata++ = st_algo_para.button_tri_thr[i];
    }
    
    for(i = 0; i < CH_COUNT; i++)//13-16
    {
        *lpdata++ = st_raw.current_buf[i];
    }

    for(i = 0; i < CH_COUNT; i++)//17-20
    {
        *lpdata++ = st_base.base[i];
    }

    for(i = 0; i < CH_COUNT; i++)//21-24
    {
        *lpdata++ = st_fv.force_val[i];
    }
    
    for(i = 0; i < CH_COUNT; i++)//25-28
    {
        *lpdata++ = st_fv.force_local[i];
    }
    
    for(i = 0; i < CH_COUNT; i++)//29-32
    {
        *lpdata++ = st_common.balance_buf[i] * 1000;
    }
    
    for(i = 0; i < PHY_CH_COUNT; i++)//33-36
    {
        *lpdata++ = st_common.temp_coef_buf[i] * 1000;
    }
    
    for(i = 0; i < MAX_TP_COUNT; i++)//37-40
    {
        *lpdata++ = st_tp_info[i].x_point;
        *lpdata++ = st_tp_info[i].y_point;
    }

    for(i = 0; i < PHY_CH_COUNT; i++)//41-44
    {
        *lpdata++ = st_common.adc_data[i];
    }

    for(i = 0; i < PHY_CH_COUNT; i++)//41-44
    {
        *lpdata++ = st_common.dac_data[i];
    }
    
    ///////////////////////////////////////////
    
    for(i = 0; i < GR_COUNT; i++)//53-54
    {
        *lpdata++ = st_alg_rule.ff_pos_tri[i];
    }

    for(i = 0; i < GR_COUNT; i++)//55-56
    {
        *lpdata++ = st_alg_rule.ff_pos_rel[i];
    }

	for(i = 0; i < GR_COUNT; i++)//60-61
    {
        *lpdata++ = st_alg_rule.ff_neg_tri[i];
    }

    for(i = 0; i < GR_COUNT; i++)//62-63
    {
        *lpdata++ = st_alg_rule.ff_neg_rel[i];
    }
    
    for(i = 0; i < GR_COUNT; i++)//57-58
    {
        *lpdata++ = st_alg_rule.tf_tri[i];
    }

    for(i = 0; i < GR_COUNT; i++)//59-60
    {
        *lpdata++ = st_alg_rule.tf_rel[i];
    }

	for(i = 0; i < GR_COUNT; i++)//49-50
    {
        *lpdata++ = st_alg_rule.ev_tri[i];
    }

    for(i = 0; i < GR_COUNT; i++)//51-52
    {
        *lpdata++ = st_alg_rule.ev_rel[i];
    }

    for(i = 0; i < CH_COUNT; i++)//64-67
    {
        *lpdata++ = fv_local[i];
    }
    
    for(i = 0; i < BU_COUNT; i++)//68-69
    {
        *lpdata++ = qc_coef[i] * 1000;
    }

    for(i = 0; i < BU_COUNT; i++)//70-71
    {
        *lpdata++ = slide_coef[i] * 1000;
    }
	
    *lpdata++ = ev_rel_dbg;	//72
    
    *lpdata++ = ((st_tp_info[0].state) << 8) + st_tp_info[1].state; //73
    *lpdata++ = st_sys_ctr.period; //74
    *lpdata++ = st_config.modify_times; //75
    
    *lpdata++ = st_common.temprature_cur * 10; //11
	*lpdata++ = st_common.temprature_init * 10; //77
    *lpdata++ = st_common.temprature_init_bat * 10; //78
	*lpdata++ = st_common.temprature_real * 10; //76
	
	for(i = 0; i < PHY_CH_COUNT; i++)//79-71
    {
        *lpdata++ = st_common.origin_adc_data[i];
    }

//	*lpdata++ = st_sys_ctr.dis_offset_cancel_cnt;

	for (i = 0; i < GR_COUNT; i++)
    {
		*lpdata++ = st_time_out.cnt[i];
    }

//extern s16 dbg_sum_recd[2];
//	for (i = 0; i < GR_COUNT; i++)
//    {
//		*lpdata++ = dbg_sum_recd[i];
//    }
	
	len = lpdata - (s16*)log;

	return len * sizeof(s16);
}
#endif //DEBUG_F5_FUNCTION_SELECT

#ifdef EN_NOISE_CALC_BY_FW

void dbg_mode_read_noise_val(u8 mode_select)
{
    static u8 noise_det_started = 0;
    static u16 noise_det_len = 0;
    static u16 noise_det_cnt = 0;
    static u32 noise_det_sum[PHY_CH_COUNT] = {0};
    static u16 noise_det_peak[PHY_CH_COUNT][2] = {0};  //0:max peak 1:second peak
    static s16 adc_val_pre[PHY_CH_COUNT] = {0};

    s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
    u8 i;
    s32 diff;

    if (noise_det_started)
    {
        if (++noise_det_cnt > 10) //???????10?
        {
            for (i = 0; i < PHY_CH_COUNT; i++)
            {
                diff = ABSDATA(st_common.adc_data[i], adc_val_pre[i]);
                noise_det_sum[i] += diff * diff;

                if (diff > noise_det_peak[i][0])
                {
                    noise_det_peak[i][1] = noise_det_peak[i][0];
                    noise_det_peak[i][0] = diff;
                }
                else if (diff > noise_det_peak[i][1])
                {
                    noise_det_peak[i][1] = diff;
                }

            }
        }

        if (noise_det_cnt > noise_det_len + 10)
        {
            noise_det_started = 0;

            for (i = 0; i < PHY_CH_COUNT; i++)
            {
                diff = noise_det_peak[i][0];
                noise_det_sum[i] -= diff * diff;

                *lpdata++ = noise_det_peak[i][1];
                *lpdata++ = noise_det_sum[i];
                *lpdata++ = noise_det_sum[i] >> 16;
            }

            st_interaction[mode_select].rw_cound_buf[0] = PHY_CH_COUNT * 6;

//            st_sys_ctr.period = DF_HIGH_SPEED;
//            hal_timer_reset(0, st_sys_ctr.period);

        }

        st_scan_mode_opt.enter_low_speed_cnt = 0;
		
    }
    else
    {
        if (2 == st_interaction[mode_select].rw_cound_buf[0])
        {
            noise_det_len = *lpdata;
            noise_det_started = 1;
            noise_det_cnt = 0;

            memset(noise_det_sum, 0, sizeof(noise_det_sum));
            memset(noise_det_peak, 0, sizeof(noise_det_peak));

            st_interaction[mode_select].rw_cound_buf[0] = 0;

//            st_sys_ctr.period = 2;
//            hal_timer_reset(0, st_sys_ctr.period);
        }
    }

    memcpy(adc_val_pre, st_common.adc_data, sizeof(st_common.adc_data));
}

#endif

#ifdef EN_RESULT_CALC_BY_FW

void dbg_mode_read_offset_res(u8 mode_select)
{
    u8 i = 0;
    s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
    u8 len;
    u16 res = 0;
    s32 offset_val;

    if(0 == st_interaction[mode_select].rw_cound_buf[0])
    {
        cmd_enter_high_speed_period();

        for (i = 0; i < PHY_CH_COUNT; i++)
        {
            offset_val = st_reg_dac_2_uvolt_coef.data[0] + (st_reg_dac_2_uvolt_coef.data[1] << 8);
            offset_val *= (s16)(st_reg_dac_value.data[i * 2] + (st_reg_dac_value.data[i * 2 + 1] << 8));

            if (offset_val > OFFSET_MAX_THR * 1000 || offset_val < OFFSET_MIN_THR * 1000)
            {
                res |= 1 << i;
            }
        }

        *lpdata++ = res;

        for (i = 0; i < PHY_CH_COUNT; i++)
        {
            *lpdata++ = (s16)(st_reg_dac_value.data[i * 2] + (st_reg_dac_value.data[i * 2 + 1] << 8));
        }

        len = lpdata - (s16 *)&st_interaction[mode_select].rw_data_buf[0];

        st_interaction[mode_select].rw_cound_buf[0] = (len << 1);
    }
}

void dbg_mode_read_noise_sub(u8 mode_select)
{
    static u8 noise_det_started = 0;
    static u16 noise_det_len = 0;
    static u16 noise_det_cnt = 0;
    static u32 noise_det_sum[PHY_CH_COUNT] = {0};
    static u16 noise_det_peak[PHY_CH_COUNT][2] = {0};  //0:max peak 1:second peak
    static s16 adc_val_pre[PHY_CH_COUNT] = {0};

    s16 *lpdata = (s16 *)&st_interaction[mode_select].rw_data_buf[0];
    u8 i;
    s32 diff;
    u8 res = 0;

    if (noise_det_started)
    {
        if (++noise_det_cnt > 10) //???????10?
        {

            for (i = 0; i < PHY_CH_COUNT; i++)
            {
                diff = ABSDATA(st_common.adc_data[i], adc_val_pre[i]);
                noise_det_sum[i] += diff * diff;

                if (diff > noise_det_peak[i][0])
                {
                    noise_det_peak[i][1] = noise_det_peak[i][0];
                    noise_det_peak[i][0] = diff;
                }
                else if (diff > noise_det_peak[i][1])
                {
                    noise_det_peak[i][1] = diff;
                }

            }
        }

        if (noise_det_cnt > noise_det_len + 10)
        {
            noise_det_started = 0;

            ////////////////////////////////////////////////////////////////////////

            lpdata++;

            for (i = 0; i < PHY_CH_COUNT; i++)
            {
                diff = noise_det_peak[i][0];
                noise_det_sum[i] -= diff * diff;

                *lpdata++ = noise_det_peak[i][1];
                *lpdata++ = noise_det_sum[i];
                *lpdata++ = noise_det_sum[i] >> 16;

                //judge peak range
                s32 tmp_val = st_reg_adc_2_uvolt_coef.data[0] + (st_reg_adc_2_uvolt_coef.data[1] << 8);
                tmp_val *= noise_det_peak[i][1];

                if (tmp_val > NOISE_PEAK_MAX_THR * 10000 || tmp_val < NOISE_PEAK_MIN_THR * 10000)
                {
                    res |= 1 << i;
                }

                float coef0 = st_reg_adc_2_uvolt_coef.data[0] + (st_reg_adc_2_uvolt_coef.data[1] << 8);

                coef0 /= 10000;

                if (noise_det_sum[i] > noise_det_len * SQUARE(NOISE_STD_MAX_THR / coef0)
                        || noise_det_sum[i] < noise_det_len * SQUARE(NOISE_STD_MIN_THR / coef0)
                   )
                {
                    res |= 1 << i;
                }
            }

            st_interaction[mode_select].rw_data_buf[0] = res;
            *lpdata++ = noise_det_len;

            st_interaction[mode_select].rw_cound_buf[0] = PHY_CH_COUNT * 6 + 4;

            if (st_interaction[mode_select].cmd_buf[0] == DEBUG_MODE_READ_CHIP_NOISE_RESULT)
            {
//                hal_afe_noise_test(false);
                ndt_raw_data_init();
                st_control_flag.chip_short_state = false;
            }

            st_sys_ctr.period = DF_HIGH_SPEED;
//            hal_timer_reset(0, st_sys_ctr.period);

        }

        st_scan_mode_opt.enter_low_speed_cnt = 0;
    }
    else
    {
        if (2 == st_interaction[mode_select].rw_cound_buf[0])
        {
            noise_det_len = *lpdata;
            noise_det_started = 1;
            noise_det_cnt = 0;

            memset(noise_det_sum, 0, sizeof(noise_det_sum));
            memset(noise_det_peak, 0, sizeof(noise_det_peak));

            st_interaction[mode_select].rw_cound_buf[0] = 0;

            if (st_interaction[mode_select].cmd_buf[0] == DEBUG_MODE_READ_CHIP_NOISE_RESULT)
            {
//                hal_afe_noise_test(true);
                ndt_raw_data_init();

                //to save time, immediately jump to correct state.
                for (int i = 0; i < PHY_CH_COUNT; i++)
                {
//                    hal_dac_set(i, 0);
                }

                st_control_flag.chip_short_state = true;
                st_sys_ctr.dis_offset_cancel_cnt = 0;
            }

//            st_sys_ctr.period = 2;
//            hal_timer_reset(0, st_sys_ctr.period);
        }
    }

    memcpy(adc_val_pre, st_common.adc_data, sizeof(st_common.adc_data));
}

void dbg_mode_read_chip_noise_res(u8 mode_select)
{
    dbg_mode_read_noise_sub(mode_select);
}

void dbg_mode_read_noise_res(u8 mode_select)
{
    dbg_mode_read_noise_sub(mode_select);
}

#endif



s16 raw_reshape(u8 i)
{
    s16 ret = 0;

    if (i > CH_COUNT - 1)
    {
        return 0;
    }

    // st_common.hol_balance_buf[i] = 1;
    switch (i)
    {

        case 0:
            st_common.hol_balance_buf[i] = st_common.temp_coef_buf[i] * get_tp_coef(i);
            ret = st_adc.val[0];
            break;

        case 1:
            st_common.hol_balance_buf[i] = st_common.temp_coef_buf[i] * get_tp_coef(i);
            ret = st_adc.val[1];
            break;

        case 2:
            st_common.hol_balance_buf[i] = st_common.temp_coef_buf[i] * get_tp_coef(i);
            ret = st_adc.val[2];
            break;

        case 3:
            st_common.hol_balance_buf[i] = st_common.temp_coef_buf[i] * get_tp_coef(i);
            ret = st_adc.val[3];
            break;

        default:
            st_common.hol_balance_buf[i] = 1;
            ret = 0;
            break;
		
    }

    st_common.balance_buf[i] = st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];

    return ret;
}

void raw_overflow_check_indv(void)
{
    u8 i, m, k;

    for (i = 0; i < PHY_CH_COUNT; i++)
    {
        if (ABS(st_adc.accumulator[i]) > 0x7000
                || ((ABS(st_raw.current_buf[0]) > 0x7000)
                    && (0 == i || 1 == i)
                   )
                || ((ABS(st_raw.current_buf[1]) > 0x7000)
                    && (2 == i || 3 == i)
                   )
                || ((ABS(st_raw.current_buf[2]) > 0x7000)
                    && (4 == i || 5 == i)
                   )
                || ((ABS(st_raw.current_buf[3]) > 0x7000)
                    && (6 == i || 7 == i)
                   )
           )
        {
            k = i / 2;

            for(m = 0; m < SAMPLE_COUNT; m++)
            {
                st_raw.sample_buf[k][m] -= st_adc.accumulator[i];
            }

            st_raw.current_buf[k] -= st_adc.accumulator[i];

            st_base.base[k] -= st_adc.accumulator[i];
            st_base.fbase[k] -= (s32)(1024 * st_adc.accumulator[i]);

            st_adc.accumulator[i] = 0;
        }
    }
}


