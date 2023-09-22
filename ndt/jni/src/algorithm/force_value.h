/*************************************************
* NDT FORCE VALUE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __FORCE_VALUE_H
#define __FORCE_VALUE_H

#include "compile_option.h"

typedef enum
{
    em_un_stable = 0,
    em_mildly_stable,
    em_normal_stable,
    em_strictly_stable,
} EM_FORCE_STABLE_STATE;


#define FORCE_ERR_STATE_NONE          (0)
#define FORCE_ERR_STATE_HIGH_TEMP     (0X01)
#define FORCE_ERR_STATE_LOW_TEMP      (0X02)
#define FORCE_ERR_STATE_QUICK_CLICK   (0X04)
#define FORCE_ERR_STATE_EXTREMELY_HIGH_TEMP (0x80)
#define FORCE_ERR_STATE_SERIOUS_NOISE (0X100)
#define FORCE_ERR_STATE_KNOCKED       (0X200)

#define FORCE_ERR_MASK                (0XFF00)
#define FORCE_STATE_MASK			  (0XFF)

typedef struct
{
    s8 max_ch_pre[GR_COUNT]; //used for channel switch

    u8 force_rank[CH_COUNT];
    u8 peak_rank[CH_COUNT];
    u8 peak_pre_rank[CH_COUNT];
    u8 diff_rank[CH_COUNT];

    AdcType diff_val[CH_COUNT];
    AdcType force_val[CH_COUNT];

    AdcType force_peak[CH_COUNT]; // max force
    AdcType force_valley[CH_COUNT]; // min force
    AdcType force_peak_pre[CH_COUNT]; // max force in last period

    AdcType force_local[CH_COUNT]; // local force value
    s32     slp_sum16[CH_COUNT];   //sum slp of the latest 16 frames

    EM_FORCE_STABLE_STATE stable_state[GR_COUNT];
    EM_FORCE_STABLE_STATE ss_ch[CH_COUNT];
    AdcType max_force[GR_COUNT];
    u16 err_state[GR_COUNT];

} ST_FORCE_VALUE;

extern ST_FORCE_VALUE st_fv;

///////////////////////////////////////////////////////////////////////////////////////////////

extern void force_value_init(void);
extern void force_value(void);
extern s16 calc_min_slp_prd(u8 ch, u8 len);
///////////////////////////////////////////////////////////////////////////////////////////////

#endif /* __FORCE_VALUE_H */

