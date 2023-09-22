#include "data_parse.h"


void hal_channel_check(uint8_t ch_count)
{
	uint8_t i = 0;
	uint16_t buff = 0;
	uint16_t FailedFlag = 0;

//	hal_afe_enable();

	for(i = 0; i < ch_count; i++)
	{
//		if(FailedFlag == 0)
		{
			buff |= (0x1 << i);
		}
	}

//	hal_afe_disable();

	st_hal_platform.ch_flag = buff;
	st_reg_ch_conn_status.data[0] = ~st_hal_platform.ch_flag;
	st_reg_ch_conn_status.data[1] = ~st_hal_platform.ch_flag >> 8;
}




