/*************************************************
* NDT BUTTON DETECT
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "button_detect.h"
#include "algorithm_init.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ST_BUTTON_DETECT st_button_det;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if 0

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void _button_detect(uint8_t group_index, uint8_t bu_index)
{
    uint16_t ch_start = st_sensor.ch_start[group_index];

    #ifdef POSITION_FOR_BUTTON
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(true == st_pos_detect.cur_flag[group_index])
    {
        if(st_pos_detect.cur_pos[group_index] >= st_button_detect.pos_start_ptr[bu_index]
                && st_pos_detect.cur_pos[group_index] <= st_button_detect.pos_end_ptr[bu_index]
          )
        {
            if(false == st_button_detect.status[bu_index])
            {
                if(st_force_value.force_val[ch_start + st_force_value.force_rank[ch_start]] > st_button_detect.tri_thr_ptr[bu_index])
                {
                    st_button_detect.status[bu_index] = true;
                }

            }
            else
            {
                if(st_force_value.force_val[ch_start + st_force_value.force_rank[ch_start]] < st_button_detect.rel_thr_ptr[bu_index])
                {
                    st_button_detect.status[bu_index] = false;
                }
            }
        }
        else
        {
            st_button_detect.status[bu_index] = false;
        }
    }
    else
    {
        st_button_detect.status[bu_index] = false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #else
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(st_button_detect.ch_start_ptr[bu_index] <= (ch_start + st_force_value.force_rank[ch_start])
            && st_button_detect.ch_end_ptr[bu_index] >= (ch_start + st_force_value.force_rank[ch_start])
      )
    {
        if(false == st_button_detect.status[bu_index])
        {
            if(st_force_value.force_val[ch_start + st_force_value.force_rank[ch_start]] > st_button_detect.tri_thr_ptr[bu_index])
            {
                st_button_detect.status[bu_index] = true;
            }
        }
        else
        {
            if(st_force_value.force_val[ch_start + st_force_value.force_rank[ch_start]] < st_button_detect.rel_thr_ptr[bu_index])
            {
                st_button_detect.status[bu_index] = false;
            }
        }
    }
    else
    {
        st_button_detect.status[bu_index] = false;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    #endif /* End of POSITION_FOR_BUTTON */

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    	hal_log_info("[%2d - %2d]  %2d  |", st_button_detect.ch_start_ptr[bu_index],
    					    st_button_detect.ch_end_ptr[bu_index],
    					    (ch_start + st_force_value.force_rank[ch_start]));
    */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_event_updata(void)
{
    uint8_t i = 0;
    uint8_t event_val = 0x00;

    for(i = 0; i < BU_COUNT; i++)
    {
        if(st_button_detect.status[i] == true)
        {
            event_val |= (1 << (7 - i));
        }
    }

    st_reg_edge_event.data[1] = event_val;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_detect(void)
{
    uint8_t i = 0;
    uint8_t j = 0;

    for(i = 0 ; i < GR_COUNT; i++)
    {
        if((true == st_touch_flag.current_flag[i]) && (0 != st_sensor.bu_count[i]))
        {
            for(j = st_sensor.bu_start[i]; j < st_sensor.bu_end[i]; j++)
            {
                _button_detect(i, j);
            }
        }
        else
        {
            for(j = st_sensor.bu_start[i]; j < st_sensor.bu_end[i]; j++)
            {
                st_button_detect.status[j] = false;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    button_event_updata();

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
    	for(i = 0; i < BU_COUNT; i++)
    	{
    		hal_log_info("%2d |", st_button_detect.status[i]);
    	}

    	hal_log_info("\r\n");
    */
    /////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void button_detect_init(void)
{
    uint8_t i = 0;

    for(i = 0 ; i < BU_COUNT; i++)
    {
        st_button_detect.status[i] = false;
    }

    /*
    	st_button_detect.tri_thr_ptr = &(st_algo_para.button_tri_thr[0]);
    	st_button_detect.rel_thr_ptr = &(st_algo_para.button_rel_thr[0]);

    	st_button_detect.pos_start_ptr = &(st_algo_para.button_pos_start[0]);
    	st_button_detect.pos_end_ptr = &(st_algo_para.button_pos_end[0]);

    	st_button_detect.ch_start_ptr = &(st_algo_para.button_ch_start[0]);
    	st_button_detect.ch_end_ptr = &(st_algo_para.button_ch_end[0]);
    	*/
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
