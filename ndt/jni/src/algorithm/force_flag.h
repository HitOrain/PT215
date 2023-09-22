/*************************************************
* NDT FORCE FLAG
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __FORCE_FLAG_H
#define __FORCE_FLAG_H

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "compile_option.h"

typedef enum
{
    em_no_flag = 0,
    em_positive_flag,
    em_negtive_flag,
    em_wonder_rel_flag,

} EM_FORCE_FLAG_STATE;

typedef struct
{
    AdcType all_flag;
    AdcType sub_flag[GR_COUNT];

    u8 thr_coeff[GR_COUNT];  //in special situation

    u16 rel_cnt[GR_COUNT]; //mark the time after leave up
    u16 tri_cnt[GR_COUNT]; //mark the time after touch down

} ST_FORCE_FLAG;

extern ST_FORCE_FLAG st_ff;

///////////////////////////////////////////////////////////////////////////////////////////////////

extern void force_flag_init(void);
extern void force_flag_detect(void);

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* __FORCE_FLAG_H */
