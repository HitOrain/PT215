/*************************************************
* NDT ALGORITHM : ADC DATA -> RAW DATA
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "algorithm_init.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_RAW_DATA st_raw;
ST_DAC st_dac;
ST_ADC st_adc;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void cmd_offset_cancel(u8 state)
{
    if (1 == state)
    {
        st_control_flag.all_offset_cancel_finish_pre = false;
        st_dac.base_line_limit = BASE_LINE_NARROW;
        st_sys_ctr.dis_offset_cancel_cnt = 300000;
    }
    else
    {
        st_sys_ctr.dis_offset_cancel_cnt = 0;
    }
}

DacType ndt_calc_dac_from_adc(AdcType adc_value)
{
    return (DacType)(ABSDATA(adc_value, OFFSET_CANCEL_BASE_LINE) / st_dac.dac2adc_coef);
}

//lzy
void ndt_increment_dac_offset(AdcType *adc_data, DacType *dac_offset, u8 channel_num)
{
    //negative -->  positive  :   -2 + 5 = 3 --> (1026) - 5 ==> (1024) - [(1026) - 5] =  (3)
    if(dac_offset[channel_num] >= DAC_MIDDLE_VAL)
    {
	//1026-5=1021  1023-1021=2
        dac_offset[channel_num] -= ndt_calc_dac_from_adc(adc_data[channel_num]);

        if(dac_offset[channel_num] < DAC_MIDDLE_VAL)
        {
            dac_offset[channel_num] = DAC_MIDDLE_VAL - dac_offset[channel_num];
        }
    }
    else
    {
	//positive --> positive
        dac_offset[channel_num] += ndt_calc_dac_from_adc(adc_data[channel_num]);

        if(dac_offset[channel_num] > DAC_MIDDLE_VAL - 1)
        {
            dac_offset[channel_num] = DAC_MIDDLE_VAL - 1;
        }
    }
}

//lzy
void ndt_decrement_dac_offset(AdcType *adc_data, DacType *dac_offset, u8 channel_num)
{
	//negative --> negative
	if(dac_offset[channel_num] >= DAC_MIDDLE_VAL)
	{
		dac_offset[channel_num] += ndt_calc_dac_from_adc(adc_data[channel_num]);
		
		if(dac_offset[channel_num] > (DAC_MIDDLE_VAL << 1) - 1)
		{
			dac_offset[channel_num] = ((DAC_MIDDLE_VAL << 1) - 1);
		}
	}
	else
	{
		//positive --> negative   :  3 - 5 = -2 --> (3) - 5 ==> (1024) - [(3) - 5] =  (1026)
		dac_offset[channel_num] -= ndt_calc_dac_from_adc(adc_data[channel_num]);

		if(dac_offset[channel_num] < 0)
		{
			dac_offset[channel_num] = DAC_MIDDLE_VAL - dac_offset[channel_num];
		}
	}
}

void raw_overflow_check(void)
{
    #if (!defined(USE_MIXED_RAW))
    u8 i, m;

    for (i = 0; i < PHY_CH_COUNT; i++)
    {
        if (ABS(st_adc.accumulator[i]) > 0x7000)
        {
            for(m = 0; m < SAMPLE_COUNT; m++)
            {
                st_raw.sample_buf[i][m] -= st_adc.accumulator[i];
            }

            st_raw.current_buf[i] -= st_adc.accumulator[i];

            st_base.base[i] -= st_adc.accumulator[i];
            st_base.fbase[i] -= (s32)(1024 * st_adc.accumulator[i]);

            st_adc.accumulator[i] = 0;
        }
    }

    #else
    raw_overflow_check_indv();
    #endif
}

static void sat_check(u8 i, AdcType *adc_data)
{
    if (st_common.dac_data[i]  == DAC_MIDDLE_VAL - 1 || st_common.dac_data[i] == (DAC_MIDDLE_VAL << 1) - 1)
    {
        st_dac.total_sat_times[i]++;

        if (st_dac.total_sat_times[i] >= 50)
        {
            st_dac.total_sat_times[i] = 0;
            st_hal_platform.ch_flag &= ~(1 << i);
            st_reg_ch_conn_status.data[0] = ~st_hal_platform.ch_flag;
            st_reg_ch_conn_status.data[1] = ~st_hal_platform.ch_flag >> 8;
            adc_data[i] = OFFSET_CANCEL_BASE_LINE;     //to prevent val chopping
        }
    }
    else
    {
        st_dac.total_sat_times[i] = 0;
    }
}

void ndt_check_offset_cancel_finish(AdcType *adc_data)
{
    u8 i;
    u8 j;
    s32 avg_sum;

//////////////////////////////////////////////////

    if (st_dac.total_cal_cnt_once++ >= 50)
    {
        st_control_flag.all_offset_cancel_finish = true;
    }

    if (true == st_control_flag.all_offset_cancel_finish)
    {
        st_dac.total_cal_cnt_once = 0;
    }

//////////////////////////////////////////////////

    //需要增加跳变过程记录  对应的处理，增加offset记录
    if((true == st_control_flag.all_offset_cancel_finish)
            && (false == st_control_flag.all_offset_cancel_finish_pre)
      )
    {
        st_dac.base_line_limit = BASE_LINE_WIDTH;
        st_dac.total_cali_times++;

        for (i = 0; i < PHY_CH_COUNT; i++)
        {
            sat_check(i, adc_data);

            st_adc.accumulator[i] = st_adc.val[i] - adc_data[i];

            for (j = 0; j < AVG_POWER_COUNT; j++)
            {
                st_adc.avg_buf[i][j] = adc_data[i];
            }

            #if (VIVO_AW_8686X_PT100_C1V1_GK_V2 || VIVO_AW_8686X_PT101_C1V1_SK_V1)

            /////////update I2C reg//////////////////
            if (st_common.dac_data[i] > 2047)
            {
                st_reg_dac_value.data[i * 2] = st_common.dac_data[i] - 2048;
                st_reg_dac_value.data[i * 2 + 1] = (st_common.dac_data[i] - 2048) >> 8;
            }
            else
            {
                st_reg_dac_value.data[i * 2] = -st_common.dac_data[i];
                st_reg_dac_value.data[i * 2 + 1] = (-st_common.dac_data[i]) >> 8;
            }

            #else

            /////////update I2C reg//////////////////
            if (st_common.dac_data[i] > 2047)
            {
                st_reg_dac_value.data[i * 2] = 2048 - st_common.dac_data[i];
                st_reg_dac_value.data[i * 2 + 1] = (2048 - st_common.dac_data[i]) >> 8;
            }
            else
            {
                st_reg_dac_value.data[i * 2] = st_common.dac_data[i];
                st_reg_dac_value.data[i * 2 + 1] = (st_common.dac_data[i]) >> 8;
            }

            #endif
        }

        raw_overflow_check();
    }

    if (false == st_control_flag.all_offset_cancel_finish)
    {
        st_dac.base_line_limit = BASE_LINE_NARROW;
    }
    else
    {
        for (i = 0; i < PHY_CH_COUNT; i++)
        {
            memcpy(&(st_adc.avg_buf[i][0]), &(st_adc.avg_buf[i][1]), (st_adc.avg_count - 1) * sizeof(AdcType));

            st_adc.avg_buf[i][st_adc.avg_count - 1] = adc_data[i];

            avg_sum = 0;

            #if defined(MEDIAN_FILTER)
            u8 seq[AVG_POWER_COUNT];
            sort(&st_adc.avg_buf[i][0], seq, st_adc.avg_count, 1);

            for(j = 0; j < st_adc.avg_count - 2; j++)
            {
                avg_sum += st_adc.avg_buf[i][seq[1 + j]];
            }

            st_adc.val[i] = avg_sum / (st_adc.avg_count - 2) + st_adc.accumulator[i];
            #else

            for(j = 0; j < st_adc.avg_count; j++)
            {
                avg_sum += st_adc.avg_buf[i][j];
            }

            st_adc.val[i] = avg_sum / st_adc.avg_count + st_adc.accumulator[i];
            #endif
			
        }
    }

    st_control_flag.all_offset_cancel_finish_pre = st_control_flag.all_offset_cancel_finish;
}

void ndt_each_ch_offset_cancel(AdcType *adc_data, DacType *dac_offset, u8 channel_num)
{
    u8 i = 0;

    st_control_flag.all_offset_cancel_finish = true;

    if(adc_data[channel_num] > (OFFSET_CANCEL_BASE_LINE + st_dac.base_line_limit))
    {
        st_control_flag.each_offset_cancel_finish[channel_num] = false;
        
        ndt_decrement_dac_offset(adc_data, dac_offset, channel_num);
    }
    else if(adc_data[channel_num] < (OFFSET_CANCEL_BASE_LINE - st_dac.base_line_limit))
    {
        st_control_flag.each_offset_cancel_finish[channel_num] = false;
        
        ndt_increment_dac_offset(adc_data, dac_offset, channel_num);
    }
    else
    {
        st_control_flag.each_offset_cancel_finish[channel_num] = true;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    st_dac.cal_count[channel_num]++;

    if(st_dac.cal_count[channel_num] >= 10) //对于目标adc是848的情况，从200向848调整时，差值只有600，而非之前的2000， 因此由10调整为50.
    {
        st_control_flag.each_offset_cancel_finish[channel_num] = true;
    }

    if (true == st_control_flag.each_offset_cancel_finish[channel_num])
    {
        st_dac.cal_count[channel_num] = 0;
    }

    for(i = 0; i < PHY_CH_COUNT; i++)
    {
        st_control_flag.all_offset_cancel_finish &= st_control_flag.each_offset_cancel_finish[i];
    }

    //////////////////////////////////////////////////////////////////////////////////////
}

static bool is_all_ch_unstable(u8 channel_count)
{
    bool ret = false;
    u8 i;

    for (i = 0; i < channel_count; i++)
    {
        if (SUBABS(st_common.adc_data[i], st_adc.avg_buf[i][st_adc.avg_count - 1]) > OFFSET_CANCEL_STABLE_THR
                || (SUBABS(st_adc.avg_buf[i][st_adc.avg_count - 1], st_adc.avg_buf[i][st_adc.avg_count - 2]) > OFFSET_CANCEL_STABLE_THR
                    && st_adc.avg_count > 1
                   )
                || ((SUBABS(st_adc.avg_buf[i][st_adc.avg_count - 2], st_adc.avg_buf[i][st_adc.avg_count - 3]) > OFFSET_CANCEL_STABLE_THR
                     || SUBABS(st_common.adc_data[i], st_adc.avg_buf[i][st_adc.avg_count - 3]) > OFFSET_CANCEL_STABLE_THR)
                    && st_adc.avg_count > 2
                   )
           )
        {
            ret = true;
            break;
        }
    }

    return ret;
}

extern u16 ef_rel_cnt[GR_COUNT];
u16 skip_dynamic_calibration = 0;
void ndt_offset_cancel(char all_offset_cancel_finish[1], short dac[4])
{
    u8 i = 0;

    if (st_dac.crazy_cancel_cnt <= 100)
    {
        st_dac.crazy_cancel_cnt++;
    }

	if(true == st_control_flag.all_offset_cancel_finish_pre){
		skip_dynamic_calibration|=0x01;
	}else{
		skip_dynamic_calibration&=~(0x01);
	}

	if(st_dac.crazy_cancel_cnt > 10){
		skip_dynamic_calibration|=0x02;
	}else{
		skip_dynamic_calibration&=~(0x02);
	}

	if(st_ff.all_flag){
		skip_dynamic_calibration|=0x04;
	}else{
		skip_dynamic_calibration&=~(0x04);
	}

	if(st_ff.rel_cnt[0] < 50){
		skip_dynamic_calibration|=0x08;
	}else{
		skip_dynamic_calibration&=~(0x08);
	}
	
	if(st_ff.rel_cnt[1] < 50){
		skip_dynamic_calibration|=0x10;
	}else{
		skip_dynamic_calibration&=~(0x10);
	}
	
	if(st_reg_edge_event.data[1]){
		skip_dynamic_calibration|=0x20;
	}else{
		skip_dynamic_calibration&=~(0x20);
	}
	
	if(is_all_ch_unstable(PHY_CH_COUNT)){
		skip_dynamic_calibration|=0x40;
	}else{
		skip_dynamic_calibration&=~(0x40);
	}
	
	if(st_sys_ctr.dis_offset_cancel_cnt > 0){
		skip_dynamic_calibration|=0x80;
	}else{
		skip_dynamic_calibration&=~(0x80);
	}

    //disable _offset_cancel when touching
    if (true == st_control_flag.all_offset_cancel_finish_pre
            && st_dac.crazy_cancel_cnt > 10
            && (st_ff.all_flag
                || st_ff.rel_cnt[0] < 50
                || st_ff.rel_cnt[1] < 50
                || (st_reg_edge_event.data[1])
                || is_all_ch_unstable(PHY_CH_COUNT)
                || st_sys_ctr.dis_offset_cancel_cnt > 0
               )
       )
    {
        st_control_flag.all_offset_cancel_finish = true;
		*all_offset_cancel_finish = st_control_flag.all_offset_cancel_finish;
        ndt_check_offset_cancel_finish(st_common.adc_data);
        return;
    }

    for(i = 0; i < PHY_CH_COUNT; i ++)
    {
        ndt_each_ch_offset_cancel(st_common.adc_data, st_common.dac_data, i);
		
    }
	
	dac[0] = st_common.dac_data[0];
	dac[3] = st_common.dac_data[1];

    ndt_check_offset_cancel_finish(st_common.adc_data);
}


void ndt_all_ch_offset_cancel(AdcType *adc_data, DacType *dac_offset, u8 channel_count)
{
    u8 i = 0;

    if (st_dac.crazy_cancel_cnt <= 100)
    {
        st_dac.crazy_cancel_cnt++;
    }

    //disable _offset_cancel when touching
    if (true == st_control_flag.all_offset_cancel_finish_pre
            && st_dac.crazy_cancel_cnt > 10  //改为100的话若在100帧之内就切换到idle，会导致较长时间不生效
            && (st_ff.all_flag
//                || st_ff.rel_cnt[0] < 50
//                || st_ff.rel_cnt[1] < 50
//                || (st_reg_edge_event.data[1])
                || is_all_ch_unstable(channel_count)
                || st_sys_ctr.dis_offset_cancel_cnt > 0
               )
       )
    {
        st_control_flag.all_offset_cancel_finish = true;
        ndt_check_offset_cancel_finish(adc_data);
        return;
    }

    for(i = 0; i < channel_count; i ++)
    {
        ndt_each_ch_offset_cancel(adc_data, dac_offset, i);
    }

    ndt_check_offset_cancel_finish(adc_data);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ndt_raw_data_init(void)
{
    u8 i = 0;

    for (i = 0; i < PHY_CH_COUNT; i++)
    {
        st_adc.accumulator[i] = -OFFSET_CANCEL_BASE_LINE;
    }

    st_adc.avg_count = AVG_POWER_COUNT;

    for (i = 0; i < GR_COUNT; i++)
    {
        st_base.ok_flag[i] = 0;
        st_ff.sub_flag[i] = 0;
    }

    st_ff.all_flag = 0;

    for(i = 0; i < PHY_CH_COUNT; i++)
    {
        st_dac.cal_count[i] = 0;
    }

    st_dac.crazy_cancel_cnt = 0;

    st_dac.base_line_limit = BASE_LINE_NARROW;
    st_dac.dac2adc_coef = DAC_2_ADC_COEF; //74;

    st_output[EM_RAW_DATA].data_ptr = st_raw.current_buf;
    st_output[EM_ADC].data_ptr = st_common.adc_data;
    st_output[EM_DAC].data_ptr = st_common.dac_data;
    st_output[EM_OTHER_DEBUG_VALUE].data_ptr = st_common.debug_data;

    ///////////////////////////////////////////////////////////////////////
}

extern float get_tp_coef(u8 i);
void ndt_raw_data_get(AdcType *adc_data)
{
    u8 i = 0;

    for(i = 0; i < CH_COUNT; i++)
    {
        memcpy(&(st_raw.sample_buf[i][0]), &(st_raw.sample_buf[i][1]), (SAMPLE_COUNT - 1) * sizeof(AdcType));

        #if (!defined(USE_MIXED_RAW))
        st_raw.sample_buf[i][SAMPLE_COUNT - 1] = st_adc.val[i];
        st_common.hol_balance_buf[i] = st_common.balance_buf[i] * st_common.temp_coef_buf[i] * st_common.ap_adjust_coef_buf[i];

        #else
        st_raw.sample_buf[i][SAMPLE_COUNT - 1] = raw_reshape(i);
        st_common.hol_balance_buf[i] = st_common.temp_coef_buf[i] * get_tp_coef(i);
        st_common.balance_buf[i] = st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];
//		printf("%d\n", get_tp_coef(i));
        #endif //USE_MIXED_RAW

        st_raw.current_buf[i] = st_raw.sample_buf[i][SAMPLE_COUNT - 1];
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
