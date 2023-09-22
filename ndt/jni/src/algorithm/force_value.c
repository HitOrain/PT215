/*************************************************
* NDT FORCE VALUE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "algorithm_init.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_FORCE_VALUE st_fv;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _force_peak_record(u8 group)
{
    u8 i;

    for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        st_fv.force_peak[i] = MAX(st_fv.force_val[i], st_fv.force_peak[i]);
        st_fv.force_valley[i] = MIN(st_fv.force_val[i], st_fv.force_valley[i]);

        if (em_no_flag == st_ff.sub_flag[group])
        {
            if (0 == st_ff.rel_cnt[group])
            {
                st_fv.force_peak_pre[i] = st_fv.force_peak[i];
            }

            st_fv.force_peak[i] = 0;
            st_fv.force_valley[i] = 0;
        }
    }

    if (em_no_flag == st_ff.sub_flag[group])
    {
        st_ff.tri_cnt[group] = 0;
        st_ff.rel_cnt[group]++;

        if ((st_fv.err_state[group] & FORCE_ERR_STATE_QUICK_CLICK)
                && st_ff.rel_cnt[group] > 60
           )
        {
            st_fv.err_state[group] &= ~FORCE_ERR_STATE_QUICK_CLICK;
        }
    }
    else
    {
        if (0 == st_ff.tri_cnt[group])
        {
            if (st_ff.rel_cnt[group] < 20
                    && st_fv.force_peak_pre[st_sensor.ch_start[group] + st_fv.force_rank[st_sensor.ch_start[group]]] > TRI_THR_BU1
               )
            {
                st_fv.err_state[group] |= FORCE_ERR_STATE_QUICK_CLICK;
            }
        }

        st_ff.rel_cnt[group] = 0;
        st_ff.tri_cnt[group]++;
    }
}

void _force_value_sort(u8 group)
{
    u8 ch_start = st_sensor.ch_start[group];
    u8 ch_count = st_sensor.ch_count[group];

    sort(&(st_fv.force_val[ch_start]), &(st_fv.force_rank[ch_start]), ch_count, 1);
    sort(&(st_fv.force_peak[ch_start]), &(st_fv.peak_rank[ch_start]), ch_count, 1);
    sort(&(st_fv.force_peak_pre[ch_start]), &(st_fv.peak_pre_rank[ch_start]), ch_count, 1);
    sort(&(st_fv.diff_val[ch_start]), &(st_fv.diff_rank[ch_start]), ch_count, 1);
}

s16 _calc_local_force(u8 index)
{
#define MIN_VALID_SLP_THR  7

    u8 i;
    u8 update_num = 0;
    s16 local_sig = 0;
    s16 diff_temp;

    for (i = 1; i < LOCAL_BUFF_LENG; i++)
    {
        diff_temp = st_raw.sample_buf[index][SAMPLE_COUNT - 1] - st_raw.sample_buf[index][SAMPLE_COUNT - i];

        if (0 == local_sig)
        {
            if ((diff_temp > MIN_VALID_SLP_THR
                    && st_raw.sample_buf[index][SAMPLE_COUNT - 1] - st_raw.sample_buf[index][SAMPLE_COUNT - 1 - i - 1] > MIN_VALID_SLP_THR
                    && st_raw.sample_buf[index][SAMPLE_COUNT - 1] - st_raw.sample_buf[index][SAMPLE_COUNT - 1 - i - 2] > MIN_VALID_SLP_THR)
                    || (diff_temp < -MIN_VALID_SLP_THR
                        && st_raw.sample_buf[index][SAMPLE_COUNT - 1] - st_raw.sample_buf[index][SAMPLE_COUNT - 1 - i - 1] < -MIN_VALID_SLP_THR
                        && st_raw.sample_buf[index][SAMPLE_COUNT - 1] - st_raw.sample_buf[index][SAMPLE_COUNT - 1 - i - 2] < -MIN_VALID_SLP_THR)
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

    return (local_sig * st_common.hol_balance_buf[index]);
}

s16 calc_min_slp_prd(u8 ch, u8 len)
{
    u8 i, j;
    s16 ret = 0;
    s16 temp;
    s16 minval = MAX_S16;

    for (j = 0; j < LOCAL_BUFF_LENG; j++)
    {
        temp = 0;

        for (i = 0; i < len; i++)
        {
            temp += st_raw.sample_buf[ch][SAMPLE_COUNT - 1 - i - j] - st_raw.sample_buf[ch][SAMPLE_COUNT - 1 - i - j - len];
        }

        //寻找LOCAL_BUFF_LENG帧之内的最小斜率
        if (temp < minval)
        {
            minval = temp;
        }
    }

    ret = minval * st_common.hol_balance_buf[ch];

    ret /= len * len;

    return ret;
}

EM_FORCE_STABLE_STATE _calc_force_stable_state_sub(u8 i)
{
    u8 j;
    s16 slp_temp;
    u16 slp_cur_abs = 0;
    u16 slp_prd_abs = 0;
    u16 slp_sum_abs = 0;

    EM_FORCE_STABLE_STATE ret;

    st_fv.slp_sum16[i] = 0;

    for (j = 0; j < ORDER16; j++)
    {
        st_fv.slp_sum16[i] += st_raw.sample_buf[i][SAMPLE_COUNT - 1 - j] - st_raw.sample_buf[i][SAMPLE_COUNT - 1 - j - ORDER16];
        slp_temp = (st_raw.sample_buf[i][SAMPLE_COUNT - 1 - j] - st_raw.sample_buf[i][SAMPLE_COUNT - 1 - j - 2]);

        if (ABS(slp_temp) > slp_prd_abs)
        {
            slp_prd_abs = ABS(slp_temp);
        }
    }

    slp_prd_abs *= st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];

    slp_cur_abs = ABS(st_raw.sample_buf[i][SAMPLE_COUNT - 1] - st_raw.sample_buf[i][SAMPLE_COUNT - 1 - 2])\
                  * st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];

    slp_sum_abs = ABS(st_fv.slp_sum16[i]) * st_common.hol_balance_buf[i] / st_common.temp_coef_buf[i];

    if (slp_sum_abs < FORCE_STABLE_SUM_TH3 && slp_cur_abs < FORCE_STABLE_CUR_TH3 && slp_prd_abs < FORCE_STABLE_PRD_TH3)
    {
        ret = em_strictly_stable;
    }
    else if (slp_sum_abs < FORCE_STABLE_SUM_TH2 && slp_cur_abs < FORCE_STABLE_CUR_TH2 && slp_prd_abs < FORCE_STABLE_PRD_TH2)
    {
        ret = em_normal_stable;
    }
    else if (slp_sum_abs < FORCE_STABLE_SUM_TH1 && slp_cur_abs < FORCE_STABLE_CUR_TH1 && slp_prd_abs < FORCE_STABLE_PRD_TH1)
    {
        ret = em_mildly_stable;
    }
    else
    {
        ret = em_un_stable;
    }

    return ret;
}

void _calc_force_stable_state(u8 group)
{
    u8 i;

    st_fv.stable_state[group] = em_strictly_stable;

    // idle mode , save time , donot calc stable state
    if (st_cus_para.scan_rate_high != st_sys_ctr.period)
    {
        return;
    }

    for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        st_fv.ss_ch[i] = _calc_force_stable_state_sub(i);

        st_fv.stable_state[group] = MIN(st_fv.stable_state[group], st_fv.ss_ch[i]);
    }
}



void force_value(void)
{
    u8 i = 0;

    if(st_base.ok_flag[0] < 10)
    {
        return ;
    }

    for(i = 0; i < CH_COUNT; i++)
    {
        st_fv.diff_val[i] = st_raw.current_buf[i] - st_base.base[i];
        st_fv.force_val[i] = st_fv.diff_val[i] * st_common.hol_balance_buf[i];

        if (st_cus_para.scan_rate_high != st_sys_ctr.period) // idle mode , save time , use simple algorithm to calc force_local
        {
            st_fv.force_local[i] = st_fv.force_val[i];
        }
        else
        {
            st_fv.force_local[i] = _calc_local_force(i);
        }
    }

    for(i = 0; i < GR_COUNT; i++)
    {

        _force_peak_record(i);

        _force_value_sort(i);

        _calc_force_stable_state(i);

        st_fv.max_force[i] = st_fv.force_val[st_fv.force_rank[st_sensor.ch_start[i]] + st_sensor.ch_start[i]];
    }
}

void force_value_init(void)
{
    st_output[EM_FORCE_VALUE].data_ptr = &(st_fv.force_val[0]);
    st_output[EM_SIG_VALUE].data_ptr = &(st_fv.diff_val[0]);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

