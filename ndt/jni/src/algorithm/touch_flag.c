/*************************************************
* NDT TOUCH FLAG
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "algorithm_init.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_TOUCH_FLAG st_tf;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void _tf_peak_record(u8 group)
{
    u8 i;

    for (i = st_sensor.ch_start[group]; i < st_sensor.ch_end[group]; i++)
    {
        st_tf.sig_peak[i] = MAX(st_fv.force_val[i], st_tf.sig_peak[i]);

        if (0 == st_tf.sub_flag[group])
        {
            if (0 == st_tf.rel_cnt[group])
            {
                st_tf.sig_peak_pre[i] = st_tf.sig_peak[i];
            }

            st_tf.sig_peak[i] = 0;
        }
    }

    if (0 == st_tf.sub_flag[group])
    {
        st_tf.tri_cnt[group] = 0;
        st_tf.rel_cnt[group]++;
    }
    else
    {
        st_tf.rel_cnt[group] = 0;
        st_tf.tri_cnt[group]++;
    }
}


void _touch_flag_detect(u8 group)
{
    u8 max_ch;

    if(em_positive_flag == st_ff.sub_flag[group])
    {
        st_alg_rule.tf_tri[group] |= 0x01;


        max_ch = st_fv.force_rank[st_sensor.ch_start[group]] + st_sensor.ch_start[group];

        if (false == st_tf.sub_flag[group])
        {
            if (st_fv.force_val[max_ch] > TF_TRI_TH)
            {
                st_alg_rule.tf_tri[group] |= 0x02;
            }

            if (st_fv.force_local[max_ch] > TF_LOCAL_TH)
            {
                st_alg_rule.tf_tri[group] |= 0x04;
            }

            if (st_fv.force_val[max_ch] > st_tf.sig_peak_pre[max_ch] * 0.3
                    || st_tf.rel_cnt[group] > 70
               )
            {
                st_alg_rule.tf_tri[group] |= 0x08;
            }

            if (st_fv.force_local[max_ch] > st_tf.sig_peak_pre[max_ch] * 0.3
                    || st_tf.rel_cnt[group] > 30
               )
            {
                st_alg_rule.tf_tri[group] |= 0x10;
            }

            if (0x1f == st_alg_rule.tf_tri[group])
            {
                st_tf.sub_flag[group] = true;
            }
        }
        else
        {
            if (st_fv.force_val[max_ch] < TF_REL_TH)
            {
                st_alg_rule.tf_rel[group] |= 0x02;
            }

            if (st_fv.force_val[max_ch] - 10 < TF_REL_COEF * (st_fv.force_val[max_ch] - st_fv.force_local[max_ch] - 10))
            {
                st_alg_rule.tf_rel[group] |= 0x04;
            }

            if (st_fv.force_local[max_ch] < -TF_TRI_TH)
            {
                st_alg_rule.tf_rel[group] |= 0x08;
            }

            if (calc_min_slp_prd(max_ch, 3) < TF_REL_MIN_SLP_TH)
            {
                st_alg_rule.tf_rel[group] |= 0x10;
            }

            if (0x02 == (st_alg_rule.tf_rel[group] & 0x02)
                    || 0x1c == (st_alg_rule.tf_rel[group] & 0x1c)
               )
            {
                st_tf.sub_flag[group] = false;
            }

            //前10帧快速离手处理
            #ifdef SHORT_CLICK_REL_EN

            if  (st_tf.tri_cnt[group] <= SHORT_CLICK_REL_FRAME)
            {
                if (st_fv.force_val[max_ch] < SHORT_CLICK_REL_COEF * st_fv.force_peak[max_ch])
                {
                    st_tf.sub_flag[group] = false;
                    st_alg_rule.tf_rel[group] |= 0x20;
                }
            }

            #endif
        }
    }
    else
    {
        st_alg_rule.tf_rel[group] |= 0x01;
        st_tf.sub_flag[group] = false;
    }

    _tf_peak_record(group);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void touch_flag_detect(void)
{
    u8 i = 0;

    st_tf.all_flag = 0;

    for (i = 0; i < GR_COUNT; i++)
    {
        _touch_flag_detect(i);

        st_tf.all_flag |= st_tf.sub_flag[i] << i;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void touch_flag_init(void)
{

    st_output[EM_TOUCH_FLAG].data_ptr = &(st_tf.sub_flag[0]);

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


