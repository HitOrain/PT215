/*****************************************************************
* NDT ALGORITHM INIT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
******************************************************************/

#include "algorithm_init.h"

ST_CONTROL_FLAG st_control_flag;
ST_ALG_DBG_SUB_RULE st_alg_rule;


//////////////////////////////////////////////////////////////////////////////////////////

void ndt_algorithm_init(void)
{
    u8 i;

    st_control_flag.all_offset_cancel_finish = false;
    st_control_flag.all_raw_data_reset_finish = false;
    st_control_flag.base_line_reset_finish = false;

    ndt_raw_data_init();

    force_flag_init();
    base_line_init();
    force_value_init();
    touch_flag_init();
    position_detect_init();
    slide_init();
    time_out_init();

    for (i = 0; i < PHY_CH_COUNT; i++)
    {
        st_reg_balance_info.data[2 * i] = (u16)(st_common.balance_buf[i] * 1000);
        st_reg_balance_info.data[2 * i + 1] = (u16)(st_common.balance_buf[i] * 1000) >> 8;
    }

    /////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////////////////////
void ndt_algorithm_process(AdcType *adc_data)
{

    if(true == st_control_flag.all_offset_cancel_finish)
    {
        ndt_raw_data_get(adc_data);

        force_value();

        force_flag_detect(); //forceflag only directly related to baseline
        base_line_tracking();
        touch_flag_detect();
		
        //ndnn_protection_proc();
        //position_detect(); //0.21ms

        #ifdef SLIDE_EN
        //slide_detect();
        #endif
		
        button_detect();

        time_out_detect();
    }
}

void alg_dbg_reset(void)
{
    memset(&st_alg_rule, 0, sizeof(st_alg_rule));
}




