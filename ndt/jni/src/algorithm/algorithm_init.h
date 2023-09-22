/*************************************************
* NDT ALGORITHM INIT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __ALGORITHM_INIT_H
#define __ALGORITHM_INIT_H

#include "algorithm_all.h"
#include "raw_data.h"
#include "data_table.h"
#include "user_interface.h"
#include "reg_table.h"

///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    bool all_offset_cancel_finish;
    bool all_offset_cancel_finish_pre;
    bool each_offset_cancel_finish[PHY_CH_COUNT];

    bool all_raw_data_reset_finish;

    bool base_line_reset_finish;

    bool chip_short_state;

} ST_CONTROL_FLAG;

extern ST_CONTROL_FLAG st_control_flag;

///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    u16 button_tri_thr[BU_COUNT];
    u16 button_rel_thr[BU_COUNT];

    u16 button_pos_start[BU_COUNT];
    u16 button_pos_end[BU_COUNT];

    u16 button_pos_start_wide[BU_COUNT];
    u16 button_pos_end_wide[BU_COUNT];

    u16 button_ch_start[BU_COUNT];
    u16 button_ch_end[BU_COUNT];

    /////////////////////////////////////////////////////////////

    u16 slide_tri_thr[SA_COUNT];
    u16 slide_rel_thr[SA_COUNT];

} ST_ALGO_PARA;

extern ST_ALGO_PARA st_algo_para;

#ifndef DF_COUNT
    #define DF_COUNT 1
#endif

typedef struct
{
    u16 ff_rel_key[GR_COUNT];
    u16 ff_pos_tri[GR_COUNT];
    u16 ff_neg_tri[GR_COUNT];
    u16 ff_pos_rel[GR_COUNT];
    u16 ff_neg_rel[GR_COUNT];

    u16 tf_tri[GR_COUNT];
    u16 tf_rel[GR_COUNT];

    u16 ev_tri[BU_COUNT];
    u16 ev_rel[BU_COUNT];

    u16 df_tri[DF_COUNT];
    u16 df_rel[DF_COUNT];

} ST_ALG_DBG_SUB_RULE;

extern ST_ALG_DBG_SUB_RULE st_alg_rule;


///////////////////////////////////////////////////////////////////////////////////////////////

extern void ndt_algorithm_init(void);
extern void ndt_algorithm_process(AdcType *adc_data);
extern void alg_dbg_reset(void);
///////////////////////////////////////////////////////////////////////////////////////////////

#endif /* __ALGORITHM_INIT_H */
