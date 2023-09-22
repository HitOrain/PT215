/*************************************************
* NDT BUTTON DETECT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __BUTTON_DETECT_H
#define __BUTTON_DETECT_H

#include "algorithm_all.h"

#define BIT_VALID_BUTTON  0X01
#define BIT_TIME_OUT_BUTTON   0X02

#define BIT_EXIST_BUTTON  0XFF

typedef struct
{
    u8 all_status;
    u8 status[BU_COUNT];

} ST_BUTTON_DETECT;

extern ST_BUTTON_DETECT st_button_det;
extern void button_detect(void);


#if 0
///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    bool status[BU_COUNT];

    uint16_t *tri_thr_ptr;
    uint16_t *rel_thr_ptr;

    uint16_t *pos_start_ptr;
    uint16_t *pos_end_ptr;

    uint16_t *ch_start_ptr;
    uint16_t *ch_end_ptr;

} ST_BUTTON_DETECT;

extern ST_BUTTON_DETECT st_button_detect;

///////////////////////////////////////////////////////////////////////////////////////////////

extern void button_detect(void);
extern void button_detect_init(void);

///////////////////////////////////////////////////////////////////////////////////////////////
#endif
#endif /* __BUTTON_DETECT_H */

