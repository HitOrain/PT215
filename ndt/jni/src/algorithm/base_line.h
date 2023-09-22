/***********8************************************************
* NDT BASE LINE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
*************************************************************/
#ifndef __BASE_LINE_H
#define __BASE_LINE_H

#include "compile_option.h"

///////////////////////////////////////////////////////////////////////////////////////////////


#define BASE_DS_BUFF_GAP  25  //base down sample gap 
#define BASE_DS_BUFF_NUM  20  //base down sample number

///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    u8 ok_flag[GR_COUNT];

    AdcType base[CH_COUNT];
    s32 fbase[CH_COUNT];

    AdcType ds_buff[CH_COUNT][BASE_DS_BUFF_NUM];
    AdcType ds_max[CH_COUNT];
    AdcType ds_min[CH_COUNT];

} ST_BASE_LINE;

extern ST_BASE_LINE st_base;

///////////////////////////////////////////////////////////////////////////////////////////////
extern void reset_base_from_history_base(u8 group);
extern void base_line_init(void);
extern void base_line_tracking(void);
extern void reset_base_from_cur_raw(u8 group);
extern void reset_base_from_valley_raw(u8 group, u16 limit_val, u8 his_leng);
///////////////////////////////////////////////////////////////////////////////////////////////

#endif /* __BASE_LINE_H */
