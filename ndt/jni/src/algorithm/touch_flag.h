/*************************************************
* NDT TOUCH FLAG
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __TOUCH_FLAG_H
#define __TOUCH_FLAG_H

/////////////////////////////////////////////////////////////////////////////////////////

#include "compile_option.h"

/////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    AdcType all_flag;
    AdcType sub_flag[GR_COUNT];

    u16 rel_cnt[GR_COUNT]; //mark the time after leave up
    u16 tri_cnt[GR_COUNT]; //mark the time after touch down

    s16 sig_peak[CH_COUNT];
    s16 sig_peak_pre[CH_COUNT];
} ST_TOUCH_FLAG;

extern ST_TOUCH_FLAG st_tf;

/////////////////////////////////////////////////////////////////////////////////////////

extern void touch_flag_init(void);
extern void touch_flag_detect(void);

/////////////////////////////////////////////////////////////////////////////////////////

#endif /* __TOUCH_FLAG_H */

