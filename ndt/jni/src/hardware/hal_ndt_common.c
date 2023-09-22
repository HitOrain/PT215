/*************************************************
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#include "hal_ndt_common.h"
#include "algorithm_init.h"

////////////////////////////////////////////////////////////////////////////////////////////////////

void hal_all_adc_dac_get(AdcType *adc_data, DacType *dac_data, uint8_t ch_count)
{
	uint8_t i = 0;

	for(i = 0; i < ch_count; i++)
	{
		if(((st_hal_platform.ch_flag & st_sys_ctr.work_ch) >> i) & 0x01
			|| st_control_flag.chip_short_state
			)
		{
//			adc_data[i] = hal_adc_get(i);
//			dac_data[i] = hal_dac_get(i);
		}
		else
		{
			adc_data[i] = OFFSET_CANCEL_BASE_LINE;// to prevent _offset_cancel err
		}
	}
}

void hal_all_dac_set(DacType *dac_data, uint8_t ch_count)
{
	uint8_t i = 0;
	
	for(i = 0; i < ch_count; i++)
	{
		if(false == st_control_flag.each_offset_cancel_finish[i])
		{
//			hal_dac_set(i, dac_data[i]);
			
		}
	}
}

void hal_heart_refresh(void)
{
	u16 temp_val = st_reg_heart.data[0] + (st_reg_heart.data[1] << 8);

	temp_val++;

	st_reg_heart.data[0] = (u8)temp_val;
	st_reg_heart.data[1] = (u8)(temp_val >> 8);

	temp_val = 0x10000 - temp_val;

	st_reg_heart.data[2] = (u8)temp_val;
	st_reg_heart.data[3] = (u8)(temp_val >> 8);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

