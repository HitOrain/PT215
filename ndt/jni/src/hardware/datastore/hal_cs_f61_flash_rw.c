
#include "hal_cs_f61_flash_rw.h"

#ifdef AW_8686X

#include "algorithm_init.h"

u8 temp_buff[PROFILE_BLOCK_SEC_SIZE];

u8 flash_write(u16 block, u16 offset, u16 size, u8 *buf)
{
	if(block > BLOCK_MAX)
		return JOB_ERR;
	if((offset + size) > PROFILE_BLOCK_SEC_SIZE)
		return JOB_ERR;
	
//	hal_flash_write(PROFILE_BLOCK_SEC_START + block, offset, size, buf);

	return JOB_OK;
}

u8 flash_read(u16 block, u16 offset, u16 size, u8 *buf)
{
	if(block > BLOCK_MAX)
		return JOB_ERR;
	if((offset + size) > PROFILE_BLOCK_SEC_SIZE)
		return JOB_ERR;

//	hal_flash_read(PROFILE_BLOCK_SEC_START + block, offset, size, buf);

	return JOB_OK;
}

u8 flash_erase(u16 block)
{
	if(block > BLOCK_MAX)
		return JOB_ERR;

//	flash_erase_sector(PROFILE_BLOCK_SEC_START + block);

	return JOB_OK;
}


////////////////////////////////////////////////////////////////////////////////
//////////////////////CALIBRATION PARA START////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void reinit_cali_para(u8 *pbuf)
{
	int temp32 = 0;
	u8 i;

	for (i = 0; i < PHY_CH_COUNT; i++)
	{
		temp32 = pbuf[i*4 + 3];
		temp32 <<= 8;
		temp32 += pbuf[i*4 + 2];
		temp32 <<= 8;
		temp32 += pbuf[i*4 + 1];
		temp32 <<= 8;
		temp32 += pbuf[i*4];

		if (temp32 < 0 || temp32 > 20000)
		{
			temp32 = 1000;
		}
		
		st_common.balance_buf[i] = (float)temp32 / CHANNELCAL_DATA_GAINFACTOR;
		
		st_reg_balance_info.data[2*i] = temp32;
		st_reg_balance_info.data[2*i + 1] = temp32 >> 8;
	}
	
}

u8 init_cali_para_from_flash(void)
{
	u8 ret = JOB_ERR;
	
	st_common.temprature_init = DEFALUT_INIT_TEMP_VAL;
	st_common.temprature_init_bat = DEFALUT_INIT_TEMP_VAL;
	
	flash_read(CALIBRATION_BLOCK, 0, TOTAL_CALI_PARA_SIZE + 6, temp_buff);
			
	if (0 == calc_checksum_s16(temp_buff, TOTAL_CALI_PARA_SIZE) && (temp_buff[0] | temp_buff[1]))
	{
		reinit_cali_para(temp_buff);
		ret = JOB_OK;
		
		if (0 == calc_checksum_s16(&temp_buff[TOTAL_CALI_PARA_SIZE], 4))
		{
			st_common.temprature_init = temp_buff[TOTAL_CALI_PARA_SIZE + 1];
			st_common.temprature_init <<= 8;
			st_common.temprature_init += temp_buff[TOTAL_CALI_PARA_SIZE];
		}

		if (0 == (s8)temp_buff[TOTAL_CALI_PARA_SIZE + 4] + (s8)temp_buff[TOTAL_CALI_PARA_SIZE + 5]
			&& temp_buff[TOTAL_CALI_PARA_SIZE + 4] < 50
			&& temp_buff[TOTAL_CALI_PARA_SIZE + 4] > 0
			)
		{
			st_common.temprature_init_bat = temp_buff[TOTAL_CALI_PARA_SIZE + 4];
			st_reg_battery_temp.data[0] = temp_buff[TOTAL_CALI_PARA_SIZE + 4];
			st_reg_battery_temp.data[1] = temp_buff[TOTAL_CALI_PARA_SIZE + 5];
		}
	}
	
	return ret;
}


u8 hal_read_all_cali_from_flash(u8 *pbuf)
{
	u8 ret= JOB_ERR;
	
	flash_read(CALIBRATION_BLOCK, 0, TOTAL_CALI_PARA_SIZE, temp_buff);

	memcpy(pbuf, temp_buff, TOTAL_CALI_PARA_SIZE);
	
	if (0 == calc_checksum_s16((void *)pbuf, TOTAL_CALI_PARA_SIZE) && (pbuf[0] | pbuf[1]))
	{
		ret = JOB_OK;
	}

	return ret;
}

u8 hal_write_cali_2_flash_enforce(u8 *pbuf)
{
	u8 ret = JOB_ERR;

	if (0 == calc_checksum_s16((void *)pbuf, TOTAL_CALI_PARA_SIZE) && (pbuf[0] | pbuf[1]))
	{
//		st_common.temprature_init = (s16)hal_temperature_read();
		// hal_log_info("temprature_init: %d\r\n", st_common.temprature_init);
		pbuf[TOTAL_CALI_PARA_SIZE] = st_common.temprature_init;
		pbuf[TOTAL_CALI_PARA_SIZE + 1] = st_common.temprature_init >> 8;
		pbuf[TOTAL_CALI_PARA_SIZE + 2] = -st_common.temprature_init;
		pbuf[TOTAL_CALI_PARA_SIZE + 3] = (-st_common.temprature_init) >> 8;

		pbuf[TOTAL_CALI_PARA_SIZE + 4] = 0xff;
		pbuf[TOTAL_CALI_PARA_SIZE + 5] = 0xff;

		if ((s8)st_reg_battery_temp.data[0] + (s8)st_reg_battery_temp.data[1] == 0
			&& st_reg_battery_temp.data[0] < 50
			&& st_reg_battery_temp.data[0] > 0
			)
		{
			st_common.temprature_init_bat = (s8)st_reg_battery_temp.data[0];
			pbuf[TOTAL_CALI_PARA_SIZE + 4] = st_common.temprature_init_bat;
			pbuf[TOTAL_CALI_PARA_SIZE + 5] = -st_common.temprature_init_bat;
		}
		
		memcpy(temp_buff, pbuf, TOTAL_CALI_PARA_SIZE + 6);
		flash_erase(CALIBRATION_BLOCK);
		flash_write(CALIBRATION_BLOCK, 0, TOTAL_CALI_PARA_SIZE + 6, temp_buff);

		reinit_cali_para(pbuf);
		ret = JOB_OK;
	}

	return ret;
}


void test_cali_wr(void)
{
	u8 i;
	
	s16 cks;

	for (i = 0; i < PHY_CH_COUNT; i++)
	{
		temp_buff[4*i] = i+1;
		temp_buff[4*i+1] = 4;
		temp_buff[4*i+2] = 0;
		temp_buff[4*i+3] = 0;
	}
	
	cks = 0 - calc_checksum_s16(temp_buff, TOTAL_CALI_PARA_SIZE-2);
	temp_buff[TOTAL_CALI_PARA_SIZE-2] = cks;
	temp_buff[TOTAL_CALI_PARA_SIZE-1] = cks >> 8;

	hal_write_cali_2_flash_enforce(temp_buff);
}
////////////////////////////////////////////////////////////////////////////////
//////////////////////CALIBRATION PARA END//////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//////////////////////MODEL PARA START//////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
note: model format: len(1byte) + index(1byte) + model(2*model_cnt) + checksum(2byte)
len: total 1 model num including head & tail, namely : model + 4
*******************************************************************************/
u8 get_model_channel_block_index(u8 channel)
{
	u8 ret;

	u8 channel_2_block[MODEL_BLOCK_MAX] = {MODEL1_2_BLOCK, MODEL3_4_BLOCK,MODEL5_6_BLOCK,MODEL7_8_BLOCK,MODEL9_10_BLOCK,MODEL11_12_BLOCK,MODEL13_14_BLOCK,MODEL15_16_BLOCK};

	ret = channel_2_block[channel >> 1];

	return ret;
}

u8 init_model_from_flash(AdcType *des_buff)
{
	u8 ret = JOB_OK;
	u8 ch_cnt = 0;
	u8 i,j;
	u8 block, len;
	u16 src_offset = 0;
	u16 des_offset = 0;
		
	//traversal all channels
	for (i = 0; i < GR_COUNT; i++)
	{
		if (0 == st_sensor.model_count[i])
		{
			continue;
		}
		
		des_offset = st_sensor.model_start[i];
		
		for (j = 0; j < st_sensor.ch_count[i]; j++)
		{
			block = get_model_channel_block_index(ch_cnt);
			src_offset = (ch_cnt & 1) * ONE_MODE_MAX_SIZE;

			flash_read(block, src_offset, ONE_MODE_MAX_SIZE, temp_buff);
			len = temp_buff[0];

			if (st_sensor.model_count[i] << 1 != len - 4
				|| 0 != calc_checksum_s16(temp_buff, len)
				)
			{
				ret = JOB_ERR;
				break;
			}

			memcpy(des_buff + des_offset, temp_buff + 2, len - 4);

			ch_cnt++;
			des_offset += st_sensor.model_count[i];
		}
	}

	return ret;
}

u8 get_ch_group_index(u8 ch)
{
	u8 i;

	for (i = 0; i < GR_COUNT; i++)
	{
		if (ch >= st_sensor.ch_start[i])
		{
			return i;
		}
	}
	return 0;
}

u8 hal_read_single_model_from_flash(u8 index, u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 block;
	u8 len;
	u16 src_offset;
	u8 group;

	src_offset = ONE_MODE_MAX_SIZE * (index % 2);
	block = get_model_channel_block_index(index);
	group = get_ch_group_index(index);

	flash_read(block, src_offset, ONE_MODE_MAX_SIZE, temp_buff);
	len = temp_buff[0];
	memcpy(pbuf, temp_buff, len);

	if (st_sensor.model_count[group] << 1 != len - 4
	 )
	{
		ret |= 0x02;
	}
	if ( 0 != calc_checksum_s16(pbuf, len))
	{
		ret |= 0x04;
	}


	return ret;
}

u8 hal_write_single_model_2_flash(u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 block;
	u16 des_offset;
	u8 len;
	u8 index = pbuf[1];
	u8 group;

	len = pbuf[0];

	group = get_ch_group_index(index);

	if ((st_sensor.model_count[group] << 1) != len - 4)
	{
		ret |= 0x02;

		return ret;
	}

	block = get_model_channel_block_index(index);

	des_offset = ONE_MODE_MAX_SIZE * (index & 1);

	if (0 == calc_checksum_s16(pbuf, len))
	{
		if (0 == des_offset)
		{	
			flash_erase(block);
		}
		
		memcpy(temp_buff, pbuf, len);
		flash_write(block, des_offset, len, temp_buff);
	}
	else
	{
		ret |= 0x04;
	}
	return ret;
}

u8 hal_write_large_model_2_flash(u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 block;
	u8 len;
	u8 index = pbuf[1];
	u16 des_offset;

	len = pbuf[0];

	if (CONST_LARGE_MODEL_LEN != len - 4)
	{
		ret |= 0x02;

		return ret;
	}

	if (index > MODEL_BLOCK_MAX * 2 - 1)
	{
		ret |= 0x08;

		return ret;
	}

	block = get_model_channel_block_index(index);
	des_offset = ONE_MODE_MAX_SIZE * (index & 1);

	if (0 == calc_checksum_s16(pbuf, len))
	{
		if (0 == des_offset)
		{
			flash_erase(block);
		}
		memcpy(temp_buff, pbuf, len);
		flash_write(block, des_offset, len, temp_buff);
	}
	else
	{
		ret |= 0x04;
	}
	return ret;
}

u8 hal_read_large_model_from_flash(u8 index, u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 block;
	u8 len;
	u16 src_offset;

	if (index > MODEL_BLOCK_MAX * 2 - 1)
	{
		ret |= 0x08;
		return ret;
	}

	src_offset = ONE_MODE_MAX_SIZE * (index % 2);
	block = get_model_channel_block_index(index);

	flash_read(block, src_offset, ONE_MODE_MAX_SIZE, temp_buff);
	len = temp_buff[0];
	memcpy(pbuf, temp_buff, len);

	if (CONST_LARGE_MODEL_LEN != len - 4
	 )
	{
		ret |= 0x02;
	}
	if ( 0 != calc_checksum_s16(pbuf, len))
	{
		ret |= 0x04;
	}

	return ret;
}

u8 init_large_model_from_flash(AdcType *des_buff, u8 block_id, u8 len)
{
	u8 ret = JOB_OK;
	u8 block, len_flash;
	u16 src_offset = 0;

	if (block_id > MODEL_BLOCK_MAX * 2 - 1)
	{
		ret |= 0x08;
		return ret;
	}
		
	src_offset = ONE_MODE_MAX_SIZE * (block_id % 2);
	block = get_model_channel_block_index(block_id);

	flash_read(block, src_offset, ONE_MODE_MAX_SIZE - 1, temp_buff);
	len_flash = temp_buff[0];

	if (len > len_flash - 4
		|| 0 != calc_checksum_s16(temp_buff, len_flash)
		)
	{
		ret = JOB_ERR;
		return ret;
	}

	memcpy(des_buff, temp_buff + 2, len);

	return ret;
}

u8 hal_write_ap_test_2_flash(u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 len;

	len = pbuf[0];

	if (0 == calc_checksum_s16(pbuf, len))
	{

		flash_erase(AP_TEST_BLOCK);
		memcpy(temp_buff, pbuf, len);
		flash_write(AP_TEST_BLOCK, 0, len, temp_buff);
	}
	else
	{
		ret |= 0x04;
	}
	return ret;
}


u8 hal_read_ap_test_from_flash(u8 *pbuf)
{
	u8 ret = JOB_OK;
	u8 len;

	flash_read(AP_TEST_BLOCK, 0, ONE_MODE_MAX_SIZE, temp_buff);

	len = temp_buff[0];

	if ( 0 != calc_checksum_s16(temp_buff, len))
	{
		ret |= 0x04;
		return ret;
	}

	memcpy(pbuf, temp_buff, len);

	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//////////////////////MODEL PARA END////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void init_cfg_from_flash(void)
{

	flash_read(FW_CONFIG_BLOCK, 0, CONFIG_LENG_B, temp_buff);
			
	if (0 == calc_checksum_s16(temp_buff, CONFIG_LENG_B))
	{
		memcpy(&st_config, temp_buff, MIN(CONFIG_LENG_B, sizeof(st_config)));
	}
}

void hal_write_cfg_2_flash(void)
{
	if (0 == calc_checksum_s16((u8*)&st_config, CONFIG_LENG_B))
	{
		memcpy(temp_buff, &st_config, MIN(CONFIG_LENG_B, sizeof(st_config)));
		flash_erase(FW_CONFIG_BLOCK);
		flash_write(FW_CONFIG_BLOCK, 0, CONFIG_LENG_B, temp_buff);
	}
}

void hal_flash_para_init(void)
{
	init_cali_para_from_flash();

#ifndef USE_DEFAULT_MODEL
	init_model_from_flash(st_common.model_buf);
#endif

#ifdef USE_UG_FLASH_MODEL
	init_ug_model_from_flash();
#endif

	init_cfg_from_flash();

#ifdef TCC_TABLE_UPDATE_FUNCTION_SUPPORT    
    //set tcc table to defualt value
    init_tcc_table_poweron();
#endif //TCC_TABLE_UPDATE_FUNCTION_SUPPORT    
}

#endif
