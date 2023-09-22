/*************************************************
* NDT RAW DATA
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __RAW_DATA_H
#define __RAW_DATA_H

#include "compile_option.h"

#include "algorithm_all.h"

#include "aw8686x_data_process.h"

////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef OFFSET_CANCEL_STABLE_THR
    #define OFFSET_CANCEL_STABLE_THR  15  //when adc diff bigger than this value , disable offset cancel for a moment
#endif

#ifndef KNOCK_DET_THR
    #define KNOCK_DET_THR  40  //when adc diff bigger than this value , disable offset cancel for a moment
#endif



typedef struct
{

    AdcType current_buf[CH_COUNT];
    AdcType sample_buf[CH_COUNT][SAMPLE_COUNT];

} ST_RAW_DATA;

extern ST_RAW_DATA st_raw;

typedef struct
{

    AdcType val[PHY_CH_COUNT];
    AdcType avg_buf[PHY_CH_COUNT][AVG_POWER_COUNT];
    AdcType accumulator[PHY_CH_COUNT];
    u8 avg_count;

} ST_ADC;

extern ST_ADC st_adc;


//////////////////////////////////////////////////////////

typedef struct
{
    u8 dac2adc_coef;
    u8 cal_count[PHY_CH_COUNT];
    u16 base_line_limit;
    u16 total_cal_cnt_once;
    u16 total_cali_times;
    u16 total_sat_times[PHY_CH_COUNT];
    u16 crazy_cancel_cnt;  // just do offset cancel job & ignore nothing times
} ST_DAC;

extern ST_DAC st_dac;

////////////////////////////////////////////////////////////////////////////////////////////////
extern void cmd_offset_cancel(u8 state);

extern void ndt_all_ch_offset_cancel(AdcType *adc_data, DacType *dac_offset, u8 channel_count);
extern void ndt_offset_cancel(char all_offset_cancel_finish[1], short dac[4]);

extern void ndt_raw_data_init(void);

extern void ndt_raw_data_get(AdcType *adc_data);

////////////////////////////////////////////////////////////////////////////////////////////////

#endif	/* __RAW_DATA_H */
