/*************************************************
* NDT FORCE FLAG
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "algorithm_init.h"

ST_FORCE_FLAG st_ff;
static u8 max_ch_clear_flag = 0;  //bug fix, dylan 200903

u8 _calc_max_ch(u8 group)
{
    u8 max_ch, temp_ch1, temp_ch2;

    temp_ch1 = st_sensor.ch_start[group] + st_fv.force_rank[st_sensor.ch_start[group]];
    temp_ch2 = st_sensor.ch_start[group] + st_fv.force_rank[st_sensor.ch_start[group] + st_sensor.ch_count[group] - 1];
    max_ch = ABS(st_fv.force_val[temp_ch1]) > ABS(st_fv.force_val[temp_ch2]) ? temp_ch1 : temp_ch2;

    if ((
            #ifdef EASY_CHANGE_MAX_CH
                1
            #else
                st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1] - st_raw.sample_buf[max_ch][SAMPLE_COUNT - 3] > 0
            #endif
                && ABS(st_fv.force_val[max_ch]) > ABS(st_fv.force_val[st_fv.max_ch_pre[group]] + (FF_REL_TH >> 1))
                && ABS(st_fv.force_val[max_ch]) > ABS(st_fv.force_peak[st_fv.max_ch_pre[group]] * FF_LEAVE_COEF))
            || max_ch_clear_flag
            || em_positive_flag != st_ff.sub_flag[group]
       )
    {
        st_fv.max_ch_pre[group] = max_ch;
        max_ch_clear_flag = 0;
    }
    else
    {
        max_ch = st_fv.max_ch_pre[group];
    }

    return max_ch;
}

void _force_flag_detect(u8 group)
{
    u8 max_ch;
    s16 slpTmp, slpTmp2;
    float ff_leave_coeff = FF_LEAVE_COEF;
    u8 slp_det_delay = 0;
    u8 thr_coeff;

    max_ch = _calc_max_ch(group);

    if (em_no_flag == st_ff.sub_flag[group])
    {
        if (st_fv.force_val[max_ch] < FF_TRI_TH + 10
                && st_ff.rel_cnt[group] > 6
           )
        {
            slp_det_delay = 2;
        }

        if (st_fv.force_peak_pre[max_ch] > 300
                && st_ff.rel_cnt[group] < 50
           )
        {
            thr_coeff = st_ff.thr_coeff[group] * 2;
        }
        else
        {
            thr_coeff = st_ff.thr_coeff[group];
        }

        slpTmp = (st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1 - slp_det_delay] - st_raw.sample_buf[max_ch][SAMPLE_COUNT - 3 - slp_det_delay]) * st_common.hol_balance_buf[max_ch];
        slpTmp2 = (st_raw.sample_buf[max_ch][SAMPLE_COUNT - 2 - slp_det_delay] - st_raw.sample_buf[max_ch][SAMPLE_COUNT - 4 - slp_det_delay]) * st_common.hol_balance_buf[max_ch];
		
//      if (0 == (st_fv.err_state[group] & FORCE_ERR_MASK))
        {
            st_alg_rule.ff_pos_tri[group] |= 1;
            st_alg_rule.ff_neg_tri[group] |= 1;
        }

        if (st_fv.force_val[max_ch] > thr_coeff * FF_TRI_TH)
        {
            st_alg_rule.ff_pos_tri[group] |= 0x02;
        }

        if (slpTmp > thr_coeff * FF_SLP_TH1)
        {
            st_alg_rule.ff_pos_tri[group] |= 0x04;
        }

        if (slpTmp2 > thr_coeff * FF_SLP_TH2)
        {
            st_alg_rule.ff_pos_tri[group] |= 0x08;
        }

        if (st_fv.force_local[max_ch] > thr_coeff * FF_LOCAL_TH)
        {
            st_alg_rule.ff_pos_tri[group] |= 0x10;
        }

        #ifdef FF_TRI_LOCAL_BASE_RULE

        if ((st_raw.current_buf[max_ch] - st_base.ds_max[max_ch]) * st_common.hol_balance_buf[max_ch] > thr_coeff * (FF_TRI_TH - 10)
            #ifdef FF_HIGH_TEMP_DIS_HISTORY_BASE_LIMIT
                || st_fv.err_state[0] & FORCE_ERR_STATE_HIGH_TEMP
            #endif
           )
        {
            st_alg_rule.ff_pos_tri[group] |= 0x20;
        }

        #else
        {
            st_alg_rule.ff_pos_tri[group] |= 0x20;
        }
        #endif



        if (st_fv.force_val[max_ch] < -thr_coeff * FF_TRI_TH)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x02;
        }

        if (slpTmp < -thr_coeff * FF_SLP_TH1)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x04;
        }

        if (slpTmp2 < -thr_coeff * FF_SLP_TH2)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x08;
        }

        if (st_fv.force_local[max_ch] < -thr_coeff * FF_LOCAL_TH)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x10;
        }

        if (st_ff.rel_cnt[group] > 50)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x20;
        }

        #if (defined(DIS_NEG_FF_DET) && DIS_NEG_FF_DET)

        if (~st_fv.err_state[0] & FORCE_ERR_STATE_HIGH_TEMP)
        {
            st_alg_rule.ff_neg_tri[group] |= 0x40;
        }

        #else
        {
            st_alg_rule.ff_neg_tri[group] |= 0x40;
        }
        #endif

        if (0x3f == st_alg_rule.ff_pos_tri[group])
        {
            st_ff.sub_flag[group] = em_positive_flag;
        }
        else if (0x7f == st_alg_rule.ff_neg_tri[group])
        {
            st_ff.sub_flag[group] = em_negtive_flag;
        }
    }
    else if (em_positive_flag == st_ff.sub_flag[group])
    {
        if (st_fv.force_val[max_ch] < ff_leave_coeff * st_fv.force_peak[max_ch])
        {
            st_alg_rule.ff_pos_rel[group] |= 1;
        }

        if (st_fv.force_val[max_ch] < FF_REL_TH)
        {
            st_alg_rule.ff_pos_rel[group] |= 2;
        }

        if (st_alg_rule.ff_pos_rel[group])
        {
            st_ff.sub_flag[group] = em_wonder_rel_flag;
            max_ch_clear_flag = 1; //use this flag instead of dirrectly set the max_ch_pre val to -1, for somereason will cause buff overflow.
        }
    }
    else if (em_negtive_flag == st_ff.sub_flag[group])
    {
        if (st_fv.force_val[max_ch] > ff_leave_coeff * st_fv.force_valley[max_ch])
        {
            st_alg_rule.ff_neg_rel[group] |= 1;
        }

        if (st_fv.force_val[max_ch] > -FF_REL_TH)
        {
            st_alg_rule.ff_neg_rel[group] |= 2;
        }

        if (st_alg_rule.ff_neg_rel[group])
        {
            st_ff.sub_flag[group] = em_no_flag;
            st_alg_rule.ff_rel_key[group] |= 1;
        }
    }
    else if (em_wonder_rel_flag == st_ff.sub_flag[group])
    {
//        if (st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1] - st_raw.sample_buf[max_ch][SAMPLE_COUNT - 3] > 0)
        {
            st_ff.sub_flag[group] = em_no_flag;
            st_alg_rule.ff_rel_key[group] |= 2;
        }
    }
    else // err
    {
        st_ff.sub_flag[group] = em_no_flag;
    }

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void force_flag_detect(void)
{
    u8 i;

    st_ff.all_flag = 0;

    for (i = 0; i < GR_COUNT; i++)
    {
        // ref channel neednot judge ff / base tracking
        if (0 == st_sensor.bu_count[i])
        {
            continue;
        }

        _force_flag_detect(i);

        if (em_wonder_rel_flag != st_ff.sub_flag[i])
        {
            st_ff.all_flag |= st_ff.sub_flag[i] << (2 * i); // max support 4 groups
        }
        else
        {
            st_ff.all_flag |= 1 << (2 * i); // max support 4 groups
        }
    }

    /////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void force_flag_init(void)
{
    u8 i;
    st_output[EM_FORCE_FLAG].data_ptr = &(st_ff.sub_flag[0]);

    for (i = 0; i < GR_COUNT; i++)
    {
        st_ff.thr_coeff[i] = 1;
        st_ff.rel_cnt[i] = 100; // note: this must be set , because related to offset cancel
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

