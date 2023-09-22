/*************************************************
* NDT BASE LINE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "algorithm_init.h"


ST_BASE_LINE st_base;


void reset_base_from_cur_raw(u8 group)
{
    u8 i = 0;
    u8 j;

    for(i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        st_base.fbase[i] = (s32)st_raw.sample_buf[i][SAMPLE_COUNT - 1] << 10;
        st_base.base[i] = st_raw.sample_buf[i][SAMPLE_COUNT - 1];

        for (j = 0; j < BASE_DS_BUFF_NUM; j++)
        {
            st_base.ds_buff[i][j] = st_base.base[i];
        }
    }
}

void reset_base_from_history_base(u8 group)
{
    //#define HISTORY_BASE_LEN 5 // must small than BASE_DS_BUFF_NUM
    u8 i;
    //u8 his_base_seq[HISTORY_BASE_LEN];
    s16 median_base;

    for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        //	sort(&st_base.ds_buff[i][BASE_DS_BUFF_NUM - 1 - HISTORY_BASE_LEN], his_base_seq, HISTORY_BASE_LEN, 1);
        //	median_base = st_base.ds_buff[i][BASE_DS_BUFF_NUM - 1 - HISTORY_BASE_LEN + his_base_seq[HISTORY_BASE_LEN>>1]];

        median_base = st_base.ds_buff[i][BASE_DS_BUFF_NUM - 2];

        st_base.base[i] = median_base;
        st_base.fbase[i] = (s32)median_base << 10;
    }
}

void reset_base_from_valley_raw(u8 group, u16 limit_val, u8 his_leng)
{
    s32 temp_raw = 100000;
    u8 i, j;
    u8 ind = 0;
    u8 max_ch;
    s16 old_base;

    max_ch = st_fv.peak_pre_rank[st_sensor.ch_start[group]] + st_sensor.ch_start[group];

    his_leng = MIN(his_leng, SAMPLE_COUNT - 1);

    for (i = 0; i < his_leng; i++)
    {
        if (st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1 - i] < temp_raw)
        {
            temp_raw = st_raw.sample_buf[max_ch][SAMPLE_COUNT - 1 - i];
            ind = i;
        }
    }

    for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        old_base = st_base.base[i];
        temp_raw = st_raw.sample_buf[i][SAMPLE_COUNT - 1 - ind];
        temp_raw = MAX(old_base - limit_val, temp_raw);
        temp_raw = MIN(old_base + limit_val, temp_raw);

        st_base.fbase[i] = temp_raw << 10;
        st_base.base[i] = temp_raw;

        for (j = 0; j < BASE_DS_BUFF_NUM; j++)
        {
            st_base.ds_buff[i][j] = st_base.base[i];
        }
    }
}

void _base_down_sample_proc(u8 group)
{
    static u8 loop_cnt[GR_COUNT] = { 0 };
    u8 i, j;

    if (loop_cnt[group]++ > BASE_DS_BUFF_GAP)
    {
        loop_cnt[group] = 0;

        for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
        {
            memcpy(&st_base.ds_buff[i][0], &st_base.ds_buff[i][1], (BASE_DS_BUFF_NUM - 1) * sizeof(AdcType));
            st_base.ds_buff[i][BASE_DS_BUFF_NUM - 1] = st_base.base[i];

            st_base.ds_max[i] = MIN_S16;

            for (j = 0; j < BASE_DS_BUFF_NUM; j++)
            {
                st_base.ds_max[i] = MAX(st_base.ds_max[i], st_base.ds_buff[i][j]);
            }

            st_base.ds_min[i] = MAX_S16;

            for (j = 0; j < BASE_DS_BUFF_NUM; j++)
            {
                st_base.ds_min[i] = MIN(st_base.ds_min[i], st_base.ds_buff[i][j]);
            }
        }
    }
}

void _base_line_tracking(u8 group)
{
    u8 i;
    s32 delta_temp;
	int baseline_salpha = BASELINE_SALPHA;
	int base_change_limit = BASE_CHANGE_LIMIT;
	
	switch(st_sys_ctr.period){
		case 4000:
			baseline_salpha = BASELINE_SALPHA * 5;
			base_change_limit = BASE_CHANGE_LIMIT * 5;
		break;
		default:
			baseline_salpha = BASELINE_SALPHA;
			base_change_limit = BASE_CHANGE_LIMIT;

		break;
	}

    if(st_base.ok_flag[group] > 10)
    {
        for(i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
        {
            if (em_no_flag == st_ff.sub_flag[group])
            {
                if (st_fv.stable_state[group] != em_un_stable
                    #ifndef REL_DIS_BASE_TRACKING_CNT
                        && st_ff.rel_cnt[group] > 50
                    #else
                        && st_ff.rel_cnt[group] > REL_DIS_BASE_TRACKING_CNT
                    #endif
                   )
                {
                    delta_temp = (st_raw.sample_buf[i][SAMPLE_COUNT - 1] - (st_base.fbase[i] >> 10)) * baseline_salpha;

                    if (delta_temp > base_change_limit)
                    {
                        delta_temp = base_change_limit;
                    }

                    if (delta_temp < -base_change_limit)
                    {
                        delta_temp = -base_change_limit;
                    }

                    st_base.fbase[i] += delta_temp;
                }
            }

            st_base.base[i] = st_base.fbase[i] >> 10;
        }
    }
    else
    {
        st_base.ok_flag[group]++;
        reset_base_from_cur_raw(group);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void base_line_tracking(void)
{
    u8 i;

    for (i = 0; i < GR_COUNT; i++)
    {
        // ref channel neednot judge ff / base tracking
        if (0 == st_sensor.bu_count[i])
        {
            continue;
        }

        _base_line_tracking(i);
        _base_down_sample_proc(i);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void base_line_init(void)
{
    u8 i;

    for (i = 0; i < GR_COUNT; i++)
    {
        st_base.ok_flag[i] = 0;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
