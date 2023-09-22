/*************************************************
* NDT STANDARD HAL
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __HAL_NDT_COMMON_H
#define __HAL_NDT_COMMON_H

#include "compile_option.h"

//////////////////////////////////////////////////////////////////////

#ifdef AW_8686X

#endif

#define CHANNELCAL_DATA_GAINFACTOR  1000
#define ONE_CALI_PARA_SIZE          4
#define TOTAL_CALI_PARA_SIZE        (PHY_CH_COUNT * ONE_CALI_PARA_SIZE + 2)

#ifndef CHANNEL_CHECK_DETAIL_CNT
#define CHANNEL_CHECK_DETAIL_CNT 4
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define for Platform */

#define AP_STATUS_POWER_OFF	(0X01)
#define AP_STATUS_GAME_MODE	(0X02)
#define AP_STATUS_FACTORY_MODE	(0X04)
#define AP_STATUS_LANDSCAPE	(0X08)

typedef struct{
	u8 ch_buf[PHY_CH_COUNT];
	u16 io_buf[GP_COUNT];
	u16 ch_flag;
#ifdef CHANNEL_POWER_SWITCH
	u8 power_switch[PHY_CH_COUNT];
#endif
	bool timer_flag[TIMER_COUNT];
	bool afe_flag[AFE_COUNT];

	u8 bu_pre_event[BU_COUNT];

	u8 gpio_ap2mcu;
	u8 ap_status; 

} ST_HAL_PLATFORM;

extern s8 single_force[BU_COUNT];
extern ST_HAL_PLATFORM st_hal_platform;

////////////////////////////////////////////////////////////////////////////////////////////////////

extern void hal_platform_init(void);

////////////////////////////////////////////////////////////////////////////////////////////////////

extern void hal_init(void);

extern void hal_all_adc_dac_get(AdcType *adc_data, DacType *dac_data, u8 ch_count);

//extern void hal_static_reg_init(void);

extern void hal_all_dac_set(DacType *dac_data, u8 ch_count);

extern void hal_heart_refresh(void);
/////////////////////////////////////////////////////////////////////////////////////////////////

/* NDT LOG STANDARD APIS */
extern int hal_log_info(const char *fmt, ...);

/* NDT TIMER STANDARD APIS */
extern void hal_timer_reset(u8 timer, u32 time);

/* NDT DELAY STANDARD APIS */
extern void hal_delay_ms(u32 n);
extern void hal_delay_us(u32 n);

/* NDT FLASH STANDARD APIS */
extern u32 flash_erase_sector(u16 sectorNo);
extern u32 hal_flash_read(u16 block, u16 offset, u16 size, u8 *buf);
extern u32 hal_flash_write(u16 block, u16 offset, u16 size, u8 *buf);

extern u8 hal_read_all_cali_from_flash(u8 *pbuf);
extern u8 hal_write_cali_2_flash_enforce(u8 *pbuf);
extern u8 hal_read_single_model_from_flash(u8 index, u8 *pbuf);
extern u8 hal_write_single_model_2_flash(u8 *pbuf);

extern u8 hal_write_large_model_2_flash(u8 *pbuf);
extern u8 hal_read_large_model_from_flash(u8 index, u8 *pbuf);
extern u8 hal_write_ap_test_2_flash(u8 *pbuf);
extern u8 hal_read_ap_test_from_flash(u8 *pbuf);
extern u8 init_large_model_from_flash(AdcType *des_buff, u8 block_id, u8 len);

extern void hal_flash_para_init(void);


/* NDT WDT STANDARD APIS */
//extern void hal_wdt_refresh(void);

/* NDT TEMPERATUR SENSOR STANDARD APIS */
extern float hal_temperature_read(void);

/* NDT AFE STANDARD APIS */
extern u16 hal_adc_get(u8 pin_num);
extern u16 hal_dac_get(u8 pin_num);
extern void hal_dac_set(u8 pin_num, u16 dac_offset);

//extern void hal_wait_sample_finished(u8 ch_count);

extern void hal_pga_gain_set(u8 buf_num,  u8 pga_num, u8 gain);
extern u16 hal_pga_gain_get(u8 buf_num);

//extern void hal_channel_check(u8 ch_count);
extern void hal_afe_noise_test(bool afe_switch);

extern void hal_afe_enable(void);
extern void hal_afe_disable(void);

/* NDT POWER MODE STANDARD APIS */
extern void hal_power_control(void);

/* NDT IIC STANDARD APIS */
extern void hal_i2c_busy_error(void); // Used for CS F60

/* NDT CUSTOMER EVENT DEFINE */

extern void hal_event_init(void);
extern void hal_event_detect(void);

extern void hal_write_cfg_2_flash(void);

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CS_F71
u8 hal_get_io_reg_status(gpio_group_t gpio_group_num , uint16_t pin);
#else //CS_F71
extern u8 hal_get_io_reg_status(u16 io_pin);
#endif //CS_F71

extern void hal_end_toggle_all_io(void);


#ifdef AW_86802
extern void hal_uboot_read_fw_vresion(void);
#endif


#endif	/* __HAL_NDT_COMMON_H */
