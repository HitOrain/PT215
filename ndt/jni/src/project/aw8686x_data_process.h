/*************************************************
* NDT RAW DATA
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __AW8686X_DATA_PROCESS_H
#define __AW8686X_DATA_PROCESS_H

#include "stdio.h"
#include <unistd.h>

#include<android/log.h>
#ifndef LOG_TAG
#define LOG_TAG "ndt_alg"
#define slogd(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
//#define SHOW_DEBUG_INFO
//#define SHOW_TIME_INFO


#define TRUE 1
#define FALSE 0

#define PHY_CH_COUNT 2
#define MAX_TP_COUNT 2
#define GAME_MODE_EVENT_UPDATE 0x02

#define CH1_CALI_COEFF_FILE_UPPER "/mnt/vendor/persist/vforce/ch1_cali_coef_upper.txt"
#define CH1_CALI_COEFF_FILE_LOWER "/mnt/vendor/persist/vforce/ch1_cali_coef_lower.txt"
#define CH2_CALI_COEFF_FILE_UPPER "/mnt/vendor/persist/vforce/ch2_cali_coef_upper.txt"
#define CH2_CALI_COEFF_FILE_LOWER "/mnt/vendor/persist/vforce/ch2_cali_coef_lower.txt"

#define TEMPERATURE_INIT_FILE "/mnt/vendor/persist/vforce/cali_temperature.txt"

#define MOTOR "/sys/class/qcom-haptics/trigger"
#define NODE "/dev/ndt"


enum {
	SET_SCAN_TIME = 0x01,
	SET_DAC,
	SET_PGA,
	SET_EN_CH,
	SELF_CHECK,
	GET_ADC,
	GET_DAC,
	GET_TEMP,
	INTER_FAULT_MODE,
	INTER_MODE_STATE,
	DRIVER_READY = 0x0C,
	FW_VERSION = 0x10,
	IC_RESET,
	GET_DATA_PKG,
	GET_STATE_PKG,
	CHECK_LOG = 0x14,
	GET_PACKAGE,
	SET_TIMER,
	AW_OFFSET = 0x17,
	SCREEN_STATUS,
	ADC_UV = 0x1B,
	DAC_MV,
	FORCE_LOG = 0x21,
	CALI_CHECK = 0x24,
	AW_TEMP,
	CALI_COEF = 0x42,
	BATTERY_T = 0x53,
	AW_CALI_TEMP,
	GAME_MODE,
	REFERSH_RATE,
	STRAT_SILL = 0x5B,
	SENSOR_STATUS = 0xA4,
	FORCE_EVENT = 0xAB,
	RESPEND_AREA = 0xB6,
	MOTOR_MODE,
	GET_TP,
	CALI_LOG = 0xF4,
	EFFECT_LOG,
	LOG_MODE,
	DATA_READY_C,
	APK_MOD = 0xFC,
	FACTORY_MODE = 0x0E
	
};

typedef struct static_data {
	unsigned char version_id[4];
	unsigned char adc_conv_factor[3];
	unsigned short dac_conv_factor;
	short offset[PHY_CH_COUNT];
	unsigned short cali_coeff[PHY_CH_COUNT];
	unsigned short temperature_real[2+PHY_CH_COUNT];
	unsigned char temperature_init[4];
}STATIC_DATA;

typedef struct dynamic_data {
	short temperature_init_bat;
	unsigned char button_state[2];
	unsigned char apk_cnt;
	short force_log[PHY_CH_COUNT + 1];
	unsigned char cali_log[254];
	unsigned short selfcheck_log[PHY_CH_COUNT];
	unsigned char effect_log[254];
	short cali_coeff_pkg[2][250];
	unsigned char on_off_status[18];
	char algo_debug_info[256];
}DYNAMIC_DATA;

typedef struct ic_operation {
	short sample_rate;	
	short dac[4];	
	unsigned char pga[2]; //pag[0] 1-4/pga[1] 1-128
	char config;
	short selfcheck_cmd;
	unsigned char on_off_status_all[1 + 4 * 2 * 2];

}IC_OPERATION;

#pragma pack (1)

typedef struct tp {
	unsigned short x;
	unsigned short y;
}TP;

typedef struct apk_command {	
	unsigned char apk_mod;
	unsigned char factory_mod;
	unsigned char apk_cmd;
}APK_COMMAND;

typedef struct user_data {
	short trigger_threshold[4];
	unsigned char response_area[16];
	unsigned char game_mode;
	unsigned char refresh_rate;
	unsigned char motor_mode[2];
}USER_DATA;

typedef struct state_pkg {
	unsigned char head[0];
	APK_COMMAND apk_command;
	USER_DATA user_data;
	unsigned char screen_status;
	TP tp[2];
}STATE_PKG;

typedef struct data_pkg {
	unsigned char head[0];
	short adc[4];
	short temperature_cur;
}DATA_PKG;

typedef struct package {
	unsigned char head[0];
	STATE_PKG state_pkg;
	DATA_PKG data_pkg;
}PACKAGE;

#pragma pack ()

typedef struct ndt_algo_data
{
	PACKAGE package;
	
	STATIC_DATA static_data;
	DYNAMIC_DATA dynamic_data;
	IC_OPERATION ic_operation;
	
	char all_offset_cancel_finish;
	int offset_cancel_cnt;
	unsigned short algo_out_period;
	unsigned char timer_ctl;
	int fd_ndt;
	int fd_motor;
}NDT_ALGO_DATA;

extern int cali_log_update(void *data);
extern int effect_log_update(void *data);
extern void chip_noise_detect(unsigned char apk_mod[1]);
extern void factory_detect(unsigned char factory_mod[1]);
extern int screen_on(unsigned char screen_status);
extern int aw8686x_data_write(const unsigned char apk_cmd, void* const apk_data, unsigned char len);
extern int aw8686x_data_read(const unsigned char apk_cmd, void* apk_data, unsigned char len);
extern int aw8686x_adc_get(short adc[4]);
extern int aw8686x_dac_get(short dac[4]);
extern int aw8686x_aw_offset_cali(void);
extern int aw8686x_dac_set(short       dac[4], const char all_offset_cancel_finish);
extern int aw8686x_temperature_get(unsigned char * temperature_init, short * temperature_cur);
extern int aw8686x_scan_time_set(unsigned char refresh_rate);
extern int aw8686x_pga_set(unsigned char pga0, unsigned char pga1);
extern int aw8686x_count_read(unsigned char apk_cnt[1]);
extern int aw8686x_cmd_read(unsigned char apk_cmd[1]);
extern int aw8686x_cmd_write(unsigned char apk_cmd);
extern int aw8686x_count_write(unsigned char apk_cnt);
extern int aw8686x_event_write(unsigned char button_state[2], unsigned char game_mode);
extern int data_init(NDT_ALGO_DATA* param);
extern int node_open(char* node, int* fd);
extern int driver_ready_detect(void);
extern int aw8686x_state_pkg_get(STATE_PKG* state_pkg);
extern int aw8686x_data_pkg_get(DATA_PKG* data_pkg);
extern int aw8686x_package_get(PACKAGE* package, const char all_offset_cancel_finish);
extern int aw8686x_timer_controller(const char all_offset_cancel_finish, unsigned char timer_ctl[1]);

////////////////////////////////////////////////////////////////////////////////////////////////

#endif	/* __NDT_ALGO_PROCESS_H */

