#ifndef __TIME_OUT__H__
#define __TIME_OUT__H__

#include "compile_option.h"

typedef struct
{
    u16 cnt[GR_COUNT];
    u16 thr[GR_COUNT];

} ST_TIME_OUT;

extern ST_TIME_OUT st_time_out;
extern void time_out_init(void);
extern void time_out_detect(void);

#endif
