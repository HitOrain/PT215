/*************************************************
* NDT POSITION DETECT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include <math.h>
#include "algorithm_init.h"

ST_POS_DETECT st_pos_det;

#if (defined(DBG_MODEL_USE) && DBG_MODEL_USE)
    s16 dbg_model_init[10];
#endif

float my_sqrt(int data)
{
    float x = (float)data;
    float xhalf = 0.5f * x;
    int i = *(int*)&x; 
    i = 0x5f375a86 - (i>>1); 
    x = *(float*)&i;
    x = x*(1.5f-xhalf*x*x); 
    x = x*(1.5f-xhalf*x*x); 
    x = x*(1.5f-xhalf*x*x);

    return 1/x;
}

void model_sf_normalize(u8 group)
{
    u8 i, j;
    u16 model_addr=0;
    s32 temp_sum=0;
    float inv_temp=0.0;

    for (i = 0; i < st_sensor.model_count[group]; i++)
    {
        temp_sum = 0;
        model_addr = st_sensor.model_start[group] + i;

        for (j = 0; j < st_sensor.ch_count[group]; j++)
        {
            temp_sum += SQUARE(st_common.model_buf[model_addr]);
            model_addr += st_sensor.model_count[group];
        }

        inv_temp = my_sqrt(temp_sum); //inv

        #if (defined(ACTUAL_FORCE_CALC_EN) && ACTUAL_FORCE_CALC_EN)
        st_common.model_inv_buf[st_sensor.model_inv_start[group] + i] = (AdcType)inv_temp;
        #endif

        if (inv_temp < 300) //太小的信号不做归一处理
        {
            continue;
        }

        model_addr = st_sensor.model_start[group] + i;

        for (j = 0; j < st_sensor.ch_count[group]; j++)
        {
            #if (defined(DBG_MODEL_USE) && DBG_MODEL_USE)

            if (0 == i)
            {
                dbg_model_init[j] = st_common.model_buf[model_addr];
            }

            #endif

            temp_sum = (s32)st_common.model_buf[model_addr] << 14;

            st_common.model_buf[model_addr] = temp_sum / inv_temp;

            model_addr += st_sensor.model_count[group];
        }
    }
}

///////row : ch_count * (ch_count - 1)
///////col : ch_count
///////must call this function before model_normalize()  because here we need the unnormalized model
void model_df_creat_normalize(u8 group)
{
    u8 i, j, k, index = 0;
    u16 model_addr = 0, model_addr2 = 0, model_addr3 = 0;
    s32 temp_sum = 0;
    u8 pos_buff[CH_COUNT]; // mark the max sig position
    s16 temp_max = 0;
    float inv_temp = 0.0;

    /////err model length
    if ((st_sensor.model_df_count[group] << 1) != st_sensor.ch_count[group] * (st_sensor.ch_count[group] - 1))
    {
        return;
    }

    /////find max sig position of each channel
    for (i = 0; i < st_sensor.ch_count[group]; i++)
    {
        temp_max = MIN_S16;

        model_addr = st_sensor.model_start[group] + i * st_sensor.model_count[group];  //////////check asm code u8*u8

        for (j = 0; j < st_sensor.model_count[group]; j++)
        {
            if (st_common.model_buf[model_addr] > temp_max)
            {
                temp_max = st_common.model_buf[model_addr];
                pos_buff[i] = j;
            }

            model_addr++;
        }
    }

    /////creat inittial double finger model
    for (i = 0; i < st_sensor.ch_count[group]; i++)
    {
        for (j = i + 1; j < st_sensor.ch_count[group]; j++, index++)
        {
            model_addr2 = st_sensor.model_start[group] + pos_buff[i];
            model_addr3 = st_sensor.model_start[group] + pos_buff[j];
            model_addr = st_sensor.model_df_start[group] + index;

            for (k = 0; k < st_sensor.ch_count[group]; k++)
            {
                st_common.model_df_buf[model_addr] = st_common.model_buf[model_addr2] + st_common.model_buf[model_addr3];
                model_addr += st_sensor.model_df_count[group];
                model_addr2 += st_sensor.model_count[group];
                model_addr3 += st_sensor.model_count[group];
            }
        }
    }

    /////normalize double finger model
    for (i = 0; i < st_sensor.model_df_count[group]; i++)
    {
        temp_sum = 0;
        model_addr = st_sensor.model_df_start[group] + i;

        for (j = 0; j < st_sensor.ch_count[group]; j++)
        {
            temp_sum += SQUARE(st_common.model_df_buf[model_addr]);
            model_addr += st_sensor.model_df_count[group];
        }

        inv_temp = my_sqrt(temp_sum); //inv

        model_addr = st_sensor.model_df_start[group] + i;

        for (j = 0; j < st_sensor.ch_count[group]; j++)
        {
            temp_sum = (s32)st_common.model_df_buf[model_addr] << 14;

            st_common.model_df_buf[model_addr] = temp_sum / inv_temp;

            model_addr += st_sensor.model_df_count[group];
        }
    }
}

void position_detect_init(void)
{
    u8 i;

    st_output[EM_POSITION].data_ptr = &(st_pos_det.cur_pos[0]);

    for(i = 0; i < GR_COUNT; i++)
    {
        st_pos_det.cur_pos[i] = UNVALID_POS;
        st_pos_det.cur_d_pos[i] = UNVALID_POS;
        st_pos_det.s_pos_valid_thr_rel[i] = SINGLE_POS_VALID_THRS;
        st_pos_det.s_pos_valid_thr_tri[i] = SINGLE_POS_VALID_THRM;
        st_pos_det.d_pos_valid_thr_rel[i] = DOUBLE_POS_VALID_THRS;
        st_pos_det.d_pos_valid_thr_tri[i] = DOUBLE_POS_VALID_THRM;

        model_df_creat_normalize(i); //this function must be called before model_normalize()
        model_sf_normalize(i);
    }
}

//the last data is inv, so des len is 1 more than src len.
void sig_normalize(s16 *src, s16 *des, u8 len)
{
    u8 j;
    s32 temp_sig = 0;
    u16 max_abs_temp = MIN_U16;

    //find max(abs(sig))
    for (j = 0; j < len; j++)
    {
        if (ABS(src[j]) > max_abs_temp)
        {
            max_abs_temp = ABS(src[j]);
        }
    }

    //normalize to 4096
    for (j = 0; j < len; j++)
    {
        temp_sig = (s32)src[j] << 12;

        des[j] = temp_sig / max_abs_temp;
    }

    //calc ||y||^2
    temp_sig = 0;

    for (j = 0; j < len; j++)
    {
        temp_sig += SQUARE(des[j]);
    }

    des[len] = temp_sig >> 12;

    des[len + 1] = max_abs_temp;
}

//only calc the 20 point around the max channel
u8 get_pos_rough_range(u8 max_ch)
{
    return max_ch * SINGLE_SIM_CALC_DELTA;
}

s32 calc_sig_model_mul(s16 *model, s16 *sig, u8 model_gap, u8 len)
{
    s32 ret = 0;
    u8 i;

    for (i = 0; i < len; i++)
    {
        ret += (*model) * (*sig++);

        model += model_gap;
    }

    return ret;
}

//single finger force value
s16 calc_sf_force_val(u8 group, u16 offset, s16 sig_abs_max)
{
    s32 ret = 0;

    #if (defined(ACTUAL_FORCE_CALC_EN) && ACTUAL_FORCE_CALC_EN)

    ret = st_pos_det.single_product[group] / st_common.model_inv_buf[offset];

    ret >>= 2;

    ret *= sig_abs_max;

    ret >>= 14;

    #endif

    return (s16)ret;
}

void df_filter(u8 group)
{
    u8 temp_df_pos_sub1=0, temp_df_pos_sub2=0;
    u8 i;
    u8 sum = 0;

    for (i = 0; i < st_sensor.ch_count[group]; i++)
    {
        sum += st_sensor.ch_count[group] - i - 1;

        if (sum - 1 >= st_pos_det.temp_d_pos[group])
        {
            temp_df_pos_sub1 = i;
            temp_df_pos_sub2 = st_sensor.ch_count[group] - sum + st_pos_det.temp_d_pos[group];
            break;
        }
    }

    st_pos_det.cur_d_pos[group] = st_pos_det.temp_d_pos[group];

    if (ABSDATA(temp_df_pos_sub1, st_pos_det.cur_d_pos_sub1[group]) > 1)
    {
        st_pos_det.cur_d_pos_sub1[group] = temp_df_pos_sub1;
    }

    if (ABSDATA(temp_df_pos_sub2, st_pos_det.cur_d_pos_sub2[group]) > 1)
    {
        st_pos_det.cur_d_pos_sub2[group] = temp_df_pos_sub2;
    }

}

//to judge if the event is single finger or double finger
void sf_df_judge(u8 group, s32 sig_mod)
{
    s32 single_mod = 0, double_mod = 0;
    static u8 df_cnt = 0;

    single_mod = SQUARE(st_pos_det.single_product[group] >> 12) / sig_mod;
    double_mod = SQUARE(st_pos_det.double_product[group] >> 12) / sig_mod;

    #if (defined(DBG_POS_COS_SHOW_EN) && DBG_POS_COS_SHOW_EN)
    st_pos_det.dbg_s_cos_val[group] = single_mod * 1000 >> 16;
    st_pos_det.dbg_d_cos_val[group] = double_mod * 1000 >> 16;
    #endif

    if (UNVALID_POS == st_pos_det.cur_pos[group]
            && UNVALID_POS == st_pos_det.cur_d_pos[group]
       )
    {
        if (single_mod > st_pos_det.s_pos_valid_thr_tri[group])
        {
            st_pos_det.cur_pos[group] = st_pos_det.temp_pos[group];
            df_cnt = 0;
        }
        else if (double_mod > st_pos_det.d_pos_valid_thr_tri[group]
                 && st_fv.max_force[group] > TRI_THR_DOUBLE_FINGER
                )
        {
            if (df_cnt++ > 10)
            {
                df_filter(group);
            }
        }
        else
        {
            df_cnt = 0;
        }
    }
    else if (UNVALID_POS != st_pos_det.cur_pos[group])
    {
        if (single_mod < st_pos_det.s_pos_valid_thr_rel[group])
        {
            st_pos_det.cur_pos[group] = UNVALID_POS;

            if (double_mod > st_pos_det.d_pos_valid_thr_tri[group]
                    && st_fv.max_force[group] > TRI_THR_DOUBLE_FINGER
               )
            {
                df_filter(group);
            }
        }
        else
        {
            st_pos_det.cur_pos[group] = st_pos_det.temp_pos[group];
        }
    }
    else if (UNVALID_POS != st_pos_det.cur_d_pos[group])
    {
        if (double_mod < st_pos_det.d_pos_valid_thr_rel[group]
                || st_fv.max_force[group] < TRI_THR_DOUBLE_FINGER * 0.7
           )
        {
            st_pos_det.cur_d_pos[group] = UNVALID_POS;

            if (single_mod > st_pos_det.s_pos_valid_thr_tri[group])
            {
                st_pos_det.cur_pos[group] = st_pos_det.temp_pos[group];
            }
        }
    }

    if (UNVALID_POS == st_pos_det.cur_d_pos[group])
    {
        st_pos_det.cur_d_pos_sub1[group] = UNVALID_POS;
        st_pos_det.cur_d_pos_sub2[group] = UNVALID_POS;
    }

}

void calc_sf_similarity(u8 group, s16 *sig_nor)
{
    u8 i;
    u8 model_cmp_start = 0, model_cmp_end = 0; //to reduce time
    s32 temp_product;

    model_cmp_start = get_pos_rough_range(st_fv.force_rank[st_sensor.ch_start[group]]);
    model_cmp_end = MIN(model_cmp_start + SINGLE_SIM_CALC_RANGE, st_sensor.model_count[group]);

    st_pos_det.single_product[group] = 0;

    for (i = model_cmp_start; i < model_cmp_end; i++)
    {
        temp_product = calc_sig_model_mul(&st_common.model_buf[st_sensor.model_start[group]] + i, sig_nor, st_sensor.model_count[group], st_sensor.ch_count[group]);

        if (temp_product > st_pos_det.single_product[group])
        {
            st_pos_det.single_product[group] = temp_product;
            st_pos_det.temp_pos[group] = i;
        }
    }
}

void calc_df_similarity(u8 group, s16 *sig_nor)
{
    u8 i;
    s32 temp_product=0;

    st_pos_det.double_product[group] = 0;

    for (i = 0; i < st_sensor.model_df_count[group]; i++)
    {
        temp_product = calc_sig_model_mul(&st_common.model_df_buf[st_sensor.model_df_start[group]] + i, sig_nor, st_sensor.model_df_count[group], st_sensor.ch_count[group]);

        if (temp_product > st_pos_det.double_product[group])
        {
            st_pos_det.double_product[group] = temp_product;
            st_pos_det.temp_d_pos[group] = i;
        }
    }
}

void fall_status_record(u8 group)
{
    static s16 last_single_force[GR_COUNT];

    if (st_fv.max_force[group] < last_single_force[group] - 2)
    {
        st_pos_det.fall_flag_cnt[group]++;
    }
    else
    {
        st_pos_det.fall_flag_cnt[group] = 0;
    }

    last_single_force[group] = st_fv.max_force[group];
}

void _position_detect_sub(u8 group)
{
    s16 sig_nor[MAX_CH_SUB_CNT + 2]; // normalized signal + (||y||^2 >> 12) + yabsmax

    sig_normalize(&st_fv.force_val[st_sensor.ch_start[group]], sig_nor, st_sensor.ch_count[group]);

    calc_sf_similarity(group, sig_nor); //calc single finger similarity

    st_pos_det.single_force[group] = calc_sf_force_val(group, st_pos_det.temp_pos[group] + st_sensor.model_inv_start[group], sig_nor[st_sensor.ch_count[group] + 1]);

    fall_status_record(group); //monitor if it's in hand raising status

    calc_df_similarity(group, sig_nor); //calc double finger similarity

    sf_df_judge(group, sig_nor[st_sensor.ch_count[group]]); // single finger / double finger status judge

}

void position_detect(void)
{
    u8 i;

    for (i = 0 ; i < GR_COUNT; i++)
    {
        if (st_tf.sub_flag[i]
                && st_sensor.model_count[i]  // if there is no model, donnot calc position
           )
        {
            _position_detect_sub(i);
        }
        else
        {
            st_pos_det.cur_pos[i] = UNVALID_POS;
            st_pos_det.cur_d_pos[i] = UNVALID_POS;
        }
    }
}


