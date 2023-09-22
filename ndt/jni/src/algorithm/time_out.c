

#include "algorithm_init.h"

ST_TIME_OUT st_time_out;

void time_out_init(void)
{
    u8 i;

    for (i = 0; i < GR_COUNT; i++)
    {
        st_time_out.cnt[i] = 0;
        st_time_out.thr[i] = TIME_OUT_DEFAULT_THR;
    }
}

void time_out_detect(void)
{
    u8 i;

    for (i = 0; i < GR_COUNT; i++)
    {
        if (em_no_flag != st_ff.sub_flag[i]
                || st_fv.err_state[i] & FORCE_ERR_MASK
           )
        {
            st_time_out.cnt[i] += st_sys_ctr.period;
			
            if (st_time_out.cnt[i] > st_time_out.thr[i])
            {
//            	st_control_flag.all_offset_cancel_finish = false;
//            	st_control_flag.all_offset_cancel_finish_pre = false;
            	
                st_ff.sub_flag[i] = em_no_flag;
                st_alg_rule.ff_rel_key[i] |= 8;
                reset_base_from_cur_raw(i);
                st_fv.err_state[i] &= ~FORCE_ERR_MASK;
            }
        }
        else
        {
            st_time_out.cnt[i] = 0;
        }
    }

    if (st_sys_ctr.dis_report_button_cnt > 0)
    {
        st_sys_ctr.dis_report_button_cnt -= st_sys_ctr.period;
    }

    if (st_sys_ctr.dis_report_int_cnt > 0)
    {
        st_sys_ctr.dis_report_int_cnt -= st_sys_ctr.period;
    }

    if (st_sys_ctr.io_toggle_cnt > 0)
    {
        st_sys_ctr.io_toggle_cnt -= st_sys_ctr.period;
    }

    if (st_sys_ctr.dis_offset_cancel_cnt > 0)
    {
        st_sys_ctr.dis_offset_cancel_cnt -= st_sys_ctr.period;
    }
}
