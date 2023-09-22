/*************************************************
* NDT POSITION DETECT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __POSITION_DETECT_H__
#define __POSITION_DETECT_H__

/////////////////////////////////////////////////////////////////////

#include "compile_option.h"

#define UNVALID_POS (-10)

#ifndef SINGLE_SIM_CALC_DELTA
    #define SINGLE_SIM_CALC_DELTA 10
#endif

typedef struct
{

    AdcType cur_pos[GR_COUNT];
    s8 temp_pos[GR_COUNT];
    s8 temp_d_pos[GR_COUNT];
    s8 cur_d_pos[GR_COUNT];
    s8 cur_d_pos_sub1[GR_COUNT];
    s8 cur_d_pos_sub2[GR_COUNT];
    s32 single_product[GR_COUNT];
    s32 double_product[GR_COUNT];
    s16 single_force[GR_COUNT];
    u16 fall_flag_cnt[GR_COUNT];

    u16 s_pos_valid_thr_rel[GR_COUNT];
    u16 s_pos_valid_thr_tri[GR_COUNT];

    u16 d_pos_valid_thr_rel[GR_COUNT];
    u16 d_pos_valid_thr_tri[GR_COUNT];

    #if (defined(DBG_POS_COS_SHOW_EN) && DBG_POS_COS_SHOW_EN)
    s16 dbg_s_cos_val[GR_COUNT];
    s16 dbg_d_cos_val[GR_COUNT];
    #endif

} ST_POS_DETECT;

extern ST_POS_DETECT st_pos_det;

extern void position_detect_init(void);
extern void position_detect(void);

/////////////////////////////////////////////////////////////////////

#endif /* End of __POSITION_DETECT_H__ */
