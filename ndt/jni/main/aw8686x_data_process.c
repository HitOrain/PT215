/*************************************************
* NDT MAIN LOOP
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#include "ndt_algorithm_process.h"
#include <fcntl.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

NDT_ALGO_DATA st_ndt_algo_param =
{
	.dynamic_data = 
	{
		.cali_coeff_pkg = 
		 {
			{
					 11009,  7067,	 5454,	 4169,	 3123,	 2339,	 1829,	 1706,	 1920,	 2455,	 3296,	 5676,
					 5405,	 2964,	 1984,	 1478,	 1152,	 938,	 826,	 814,	 886,	 1101,	 1509,	 2686,
					 3171,	 1625,	 1093,	 802,	 634,	 535,	 488,	 494,	 561,	 706,	 1006,	 1801,
					 2252,	 1219,	 804,	 583,	 460,	 387,	 351,	 355,	 401,	 507,	 732,	 1345,
					 1767,	 920,	 626,	 468,	 372,	 316,	 286,	 293,	 341,	 436,	 643,	 1156,
					 1563,	 862,	 579,	 434,	 345,	 297,	 269,	 276,	 316,	 398,	 577,	 1088,
					 1467,	 788,	 545,	 414,	 340,	 299,	 281,	 290,	 328,	 420,	 602,	 1101,
					 1497,	 844,	 580,	 447,	 367,	 325,	 311,	 322,	 360,	 446,	 635,	 1195,
					 1614,	 876,	 619,	 485,	 407,	 370,	 358,	 372,	 421,	 531,	 751,	 1370,
					 1837,	 1028,	 731,	 565,	 481,	 441,	 426,	 445,	 501,	 620,	 872,	 1593,
					 2129,	 1165,	 832,	 666,	 577,	 533,	 526,	 554,	 637,	 788,	 1115,	 1984,
					 2606,	 1502,	 1058,	 840,	 732,	 675,	 669,	 705,	 809,	 998,	 1413,	 2608,
					 3496,	 1844,	 1311,	 1055,	 924,	 872,	 876,	 948,	 1095,	 1403,	 1960,	 3590,
					 4691,	 2531,	 1767,	 1405,	 1224,	 1155,	 1174,	 1269,	 1496,	 1896,	 2691,	 5323,
			},
			{
					4514,	2869,	2181,	1819,	1645,	1564,	1545,	1635,	1862,	2411,	3527,	7537,
					2985,	1808,	1355,	1135,	1008,	961,	960,	1022,	1156,	1429,	2031,	4005,
					2072,	1214,	914,	763,	676,	646,	650,	693,	792,	991,	1417,	2722,
					1572,	933,	683,	562,	499,	476,	478,	510,	583,	730,	1025,	1912,
					1247,	717,	529,	436,	385,	367,	372,	404,	468,	588,	837,	1514,
					1062,	615,	445,	359,	315,	298,	301,	327,	383,	483,	681,	1265,
					924,	526,	380,	304,	268,	253,	257,	284,	334,	430,	617,	1101,
					842,	493,	347,	277,	239,	224,	229,	254,	301,	385,	558,	1022,
					825,	464,	328,	260,	224,	209,	215,	243,	292,	378,	547,	975,
					810,	477,	340,	269,	230,	212,	220,	249,	299,	386,	554,	1009,
					881,	507,	367,	298,	259,	244,	252,	281,	331,	423,	602,	1045,
					961,	582,	427,	353,	316,	305,	314,	341,	396,	492,	684,	1202,
					1161,	709,	532,	449,	410,	402,	408,	435,	490,	601,	822,	1368,
					1442,	914,	695,	592,	551,	541,	557,	592,	657,	793,	1044,	1731,
					1982,	1262,	997,	868,	808,	786,	800,	849,	932,	1115,	1468,	2330,

			}
		}
	},
	
};

static unsigned char buff[256] = {0};
static unsigned char *cmd = &buff[0];
static unsigned char *data = &buff[1];

int aw8686x_data_write(const unsigned char apk_cmd, void* const apk_data, unsigned char len)
{
	int length = 0;

	*cmd = apk_cmd;
	
	if(apk_data!=NULL)
		memcpy(data, (unsigned char*)apk_data, len);

	length = len;
	if(write(st_ndt_algo_param.fd_ndt, buff, length) < 0){
		slogd("aw8686x read data error\n");
		goto WRITE_ERROR;
	}
	memset(buff, 0, length + 1);
	return 0;

WRITE_ERROR:
	
	memset(buff, 0, length + 1);

	return -1;
}

int aw8686x_data_read(const unsigned char apk_cmd, void* apk_data, unsigned char len)
{
	int length = 0;

	*cmd = apk_cmd;
	length = len;
	if(read(st_ndt_algo_param.fd_ndt, buff, length) < 0){
		slogd("aw8686x read data error\n");
		goto READ_ERROR;
	}
	if(apk_data!=NULL)
		memcpy((unsigned char*)apk_data, data, len);

	memset(buff, 0, length + 1);
	return 0;
	
READ_ERROR:
	
	memset(buff, 0, length + 1);

	return -1;
}


static int motor_shake()
{
	int length = 1;

	*cmd = 1;
	
	if(write(st_ndt_algo_param.fd_motor, buff, length) < 0){
		slogd("motor node write error\n");
		goto WRITE_ERROR;
	}

	memset(buff, 0, length + 1);
	return 0;

WRITE_ERROR:
	
	memset(buff, 0, length + 1);
	return -1;
}


int driver_ready_detect(void)
{
	unsigned char ret = 0;
	int timeout = 60;
	do{
		aw8686x_data_read(DRIVER_READY, (void *)(&ret), sizeof(ret));
		usleep(1000*500);
		slogd("driver_ready_detect ret = %d\n", ret);
	}while(!ret && timeout-- > 0);

	if(timeout > 0) {
		return 0;
	} else {
		slogd("driver load failed\n");
		return -1;
	}

}


int nvram_data_read(char* file_name, void* data, unsigned char len)
{
	FILE *fp;
	fp = fopen(file_name, "r");
	if(fp == NULL){
		slogd("Open %s failed\n", file_name);
		goto OPEN_ERROR;
	}
	
	for (int i = 0; i < len; i++) {
		if(fscanf(fp,"%hu",&((short*)data)[i])<0) {
			slogd("persist fetch data error\n");
			goto FETCH_ERROR;
		}
	}
	
	fclose(fp);	
	return 0;
	
FETCH_ERROR:
	fclose(fp); 
	
OPEN_ERROR:
	
	return -1;
}

int node_open(char* node, int* fd)
{
	*fd = open(node, O_RDWR);
	
	if(*fd < 0){
		slogd("Open aw8686x device failed\n");
		goto OPEN_ERROR;
	}

	return 0;

OPEN_ERROR:
	return -1;
}

int aw8686x_adc_get(short adc[4])
{
	return aw8686x_data_read(GET_ADC, adc, sizeof(adc));
}

int aw8686x_dac_get(short dac[4])
{	
	char temp_buf[9];

	int res = -1;
	res = aw8686x_data_read(GET_DAC, (void*)(&temp_buf), sizeof(temp_buf));

	memcpy(dac, temp_buf, 8);
	
	return res;
}

int aw8686x_aw_offset_cali(void)
{
	unsigned char status[9];
	unsigned char* calibrate_status = &status[8];
	int timeout = 100;
	const unsigned char RECALIBRATE = 0x02;
	aw8686x_data_write(RECALIBRATE, NULL, 0); //閲嶆柊鏍″噯
	while((!(*calibrate_status)) && (timeout-- > 0))
	{
		aw8686x_data_read(GET_DAC, (void*)(&status), 9);
		usleep(1000);
	}
	
	if(timeout <= 0)
		slogd("cali timeout");
		goto TIMEOUT;

	return 0;
	
TIMEOUT:
	return -1;
	
}

int aw8686x_dac_set(short       dac[4], const char all_offset_cancel_finish)
{
	static char all_offset_cancel_finish_pre = FALSE;
	int ret = 0;
	
	if((FALSE == all_offset_cancel_finish) || //动态校准未完成
		(FALSE==all_offset_cancel_finish_pre && TRUE==all_offset_cancel_finish)//完成动态校准的那一帧，可能是正常完成，也可能是超时完成
	){
		slogd("dac: %d\t%d\n", dac[0],dac[2]);
		ret = aw8686x_data_write(SET_DAC, dac, sizeof(dac));
	}
	
	all_offset_cancel_finish_pre = all_offset_cancel_finish;

	return ret;
}

int aw8686x_timer_controller(const char all_offset_cancel_finish, unsigned char timer_ctl[1])
{
	int ret = 0;
	if(FALSE == all_offset_cancel_finish){
		if(*timer_ctl == 0){
			*timer_ctl = 1;
			ret = aw8686x_data_write(SET_TIMER, timer_ctl, 1);
		}
	} else {
		if(*timer_ctl == 1){
			*timer_ctl = 0;
			ret = aw8686x_data_write(SET_TIMER, timer_ctl, 1);
		}
	}
	return ret;
}

static int nvram_temp_read(char* file_name, unsigned char data[4], unsigned char len)
{
	short temp[2];
	if(nvram_data_read(file_name, temp, len) < 0)
		goto READ_ERROR;
	
	data[0] = (unsigned char)(temp[0]);
	data[1] = (unsigned char)(-temp[0]);
	data[2] = (unsigned char)(temp[1]);
	data[3] = (unsigned char)(-temp[1]);
	
	return 0;
	
READ_ERROR:
	return -1;
}

int aw8686x_temperature_get(unsigned char temperature_init[4], short temperature_cur[1])
{
	if(nvram_temp_read(TEMPERATURE_INIT_FILE, temperature_init, 2)){
		goto READ_ERROR;
	}
	if(aw8686x_data_read(GET_TEMP, temperature_cur, sizeof(temperature_cur))){
		goto READ_ERROR;
	}
	return 0;
	
READ_ERROR:
	return -1;
}

int aw8686x_scan_time_set(unsigned char refresh_rate)
{
	int ret = 0;
	static unsigned char refresh_rate_pre = 0;
	if(refresh_rate_pre != refresh_rate){
		refresh_rate_pre = refresh_rate;
		ret = aw8686x_data_write(SET_SCAN_TIME, &refresh_rate, sizeof(refresh_rate));
	}
	
	return ret;
}

int aw8686x_pga_set(unsigned char pga0, unsigned char pga1)
{
	st_ndt_algo_param.ic_operation.pga[0] = pga1;
	st_ndt_algo_param.ic_operation.pga[1] = pga0;
	return aw8686x_data_write(SET_PGA, (void*)(&st_ndt_algo_param.ic_operation.pga), sizeof(st_ndt_algo_param.ic_operation.pga));
}

int aw8686x_cmd_read(unsigned char apk_cmd[1])
{
	return aw8686x_data_read(LOG_MODE, apk_cmd, sizeof(apk_cmd));
}

int aw8686x_count_read(unsigned char apk_cnt[1])
{
	return aw8686x_data_read(DATA_READY_C, apk_cnt, sizeof(apk_cnt));
}

int aw8686x_cmd_write(unsigned char apk_cmd)
{
	return aw8686x_data_write(LOG_MODE, &apk_cmd, sizeof(apk_cmd));
}

int aw8686x_count_write(unsigned char apk_cnt)
{
	return aw8686x_data_write(DATA_READY_C, &apk_cnt, sizeof(apk_cnt));
}



int aw8686x_event_write(unsigned char button_state[2], unsigned char game_mode)
{

	static unsigned char previous_button_state;
	
	int ret = 0;

	/*
	if(previous_button_state != st_ndt_algo_param.dynamic_data.button_state[1]){	//按键状态发生变化
	
		if(GAME_MODE_EVENT_UPDATE == st_ndt_algo_param.state_pkg.user_data.game_mode){	//游戏模式下，单指/双指都允许报键
		
			ret = aw8686x_data_write(FORCE_EVENT, (void*)(&st_ndt_algo_param.dynamic_data.button_state), sizeof(st_ndt_algo_param.dynamic_data.button_state));

			if((previous_button_state==0 && st_ndt_algo_param.dynamic_data.button_state[1]==1)
				||(previous_button_state==2 && st_ndt_algo_param.dynamic_data.button_state[1]==3)
				||(previous_button_state==1 && st_ndt_algo_param.dynamic_data.button_state[1]==0)
				||(previous_button_state==3 && st_ndt_algo_param.dynamic_data.button_state[1]==2)
				||(previous_button_state==0 && st_ndt_algo_param.dynamic_data.button_state[1]==3)
				)
			{
					motor_shake();//上键马达振动
			}
			
		}else if(3==st_ndt_algo_param.dynamic_data.button_state[1]){	//非游戏模式-双指才允许报键
		
			ret = aw8686x_data_write(FORCE_EVENT, (void*)(&st_ndt_algo_param.dynamic_data.button_state), sizeof(st_ndt_algo_param.dynamic_data.button_state));

		}
	}
	*/


	if(previous_button_state != button_state[1]){	//报键状态发生变化
		printf("1\n");
		switch(game_mode) {
			case GAME_MODE_EVENT_UPDATE:	//游戏模式下-单指/双指-触发/离手
				ret = aw8686x_data_write(FORCE_EVENT, button_state, sizeof(button_state));
				
				if((previous_button_state==0 && button_state[1]==1)
					||(previous_button_state==2 && button_state[1]==3)
					||(previous_button_state==1 && button_state[1]==0)
					||(previous_button_state==3 && button_state[1]==2)
					||(previous_button_state==0 && button_state[1]==3)
					)
				{
					motor_shake();//上键马达振动
				}

			break;

			default:
				if(3==button_state[1]){	//非游戏模式-双指触发

					ret = aw8686x_data_write(FORCE_EVENT, button_state, sizeof(button_state));

				}else if(3==previous_button_state && 3!=button_state[1]){ //非游戏模式-双指离手
			
					button_state[1] = 0x00;
					ret = aw8686x_data_write(FORCE_EVENT, button_state, sizeof(button_state));	
				}
		}
	}
	
	previous_button_state = button_state[1];
	
	return ret;
}

int aw8686x_state_pkg_get(STATE_PKG* state_pkg)
{
	return aw8686x_data_read(GET_STATE_PKG, state_pkg->head, sizeof(STATE_PKG));
}

int aw8686x_data_pkg_get(DATA_PKG* data_pkg)
{
	return aw8686x_data_read(GET_DATA_PKG, data_pkg->head, sizeof(DATA_PKG));
}

int aw8686x_package_get(PACKAGE* package, const char all_offset_cancel_finish)
{
	if(FALSE==all_offset_cancel_finish){
		
		aw8686x_state_pkg_get(&package->state_pkg);
		return aw8686x_data_pkg_get(&package->data_pkg);
	
	} else {
		return aw8686x_data_read(GET_PACKAGE, package->head, sizeof(PACKAGE));

	}
}

int screen_on(unsigned char screen_status)
{
	if(screen_status){
		slogd("screen_status:%d\n", screen_status);
	}

	if(1 == screen_status){
		
		slogd("reset aw8686x\n");
		if(aw8686x_data_write(IC_RESET, NULL, 0)){
			slogd("AW8686X reset error\n");
			goto RESET_ERROR;
		}

		screen_status = 0;
		if(aw8686x_data_write(SCREEN_STATUS, &screen_status, sizeof(screen_status))){
			goto WRITE_ERROR;
		}
	}
	return 0;

WRITE_ERROR:

RESET_ERROR:
	return -1;
}

int data_init(NDT_ALGO_DATA* param)
{	
	int cnt;
	int ret = 0;
	
	slogd("fw_version is %d.%d.%d.%d\n", param->static_data.version_id[0], param->static_data.version_id[1], param->static_data.version_id[2], param->static_data.version_id[3]);	

	cnt = 3;
	while(aw8686x_data_write(FW_VERSION, (void*)(&param->static_data.version_id), sizeof(param->static_data.version_id)) && cnt-->0){
		slogd("FW_VERSION write error\n");
		ret = -1;
	}

	cnt = 3;
	while(aw8686x_data_write(ADC_UV, (void*)(&param->static_data.adc_conv_factor), sizeof(param->static_data.adc_conv_factor)) && cnt-->0){
		slogd("ADC_UV write error\n");
		ret = -1;
	}

	cnt = 3;
	while(aw8686x_data_write(DAC_MV, (void*)(&param->static_data.dac_conv_factor), sizeof(param->static_data.dac_conv_factor)) && cnt-->0){
		slogd("DAC_MV write error\n");
		ret = -1;
	}
	
	cnt = 3;
	while(aw8686x_data_write(STRAT_SILL, (void*)(&param->package.state_pkg.user_data.trigger_threshold), sizeof(param->package.state_pkg.user_data.trigger_threshold)) && cnt-->0){
		slogd("STRAT_SILL write error\n");
		ret = -1;
	}
	
	cnt = 3;
	while(aw8686x_data_write(MOTOR_MODE, (void*)(&param->package.state_pkg.user_data.motor_mode), sizeof(param->package.state_pkg.user_data.motor_mode)) && cnt-->0){
		slogd("MOTOR_MODE write error\n");
		ret = -1;
	}
	cnt = 3;

	while(aw8686x_data_write(GAME_MODE, (void*)(&param->package.state_pkg.user_data.game_mode), sizeof(param->package.state_pkg.user_data.game_mode)) && cnt-->0){
		slogd("GAME_MODE write error\n");
		ret = -1;
	}
	cnt = 3;

	while(aw8686x_data_write(AW_CALI_TEMP, (void*)(&param->static_data.temperature_init), sizeof(param->static_data.temperature_init)) && cnt-->0){
		slogd("AW_CALI_TEMP write error\n");
		ret = -1;
	}
	cnt = 3;

	while(aw8686x_data_write(FACTORY_MODE, (void*)(&param->package.state_pkg.apk_command.factory_mod), sizeof(param->package.state_pkg.apk_command.factory_mod)) && cnt-->0){
		slogd("FACTORY_MODE write error\n");
		ret = -1;
	}
	cnt = 3;

	while(aw8686x_data_write(SET_TIMER, (void*)(&param->timer_ctl), sizeof(param->timer_ctl)) && cnt-->0){
		slogd("SET_TIMER write error\n");
		ret = -1;
	}
	cnt = 3;
	while(nvram_data_read(CH1_CALI_COEFF_FILE_UPPER, &param->dynamic_data.cali_coeff_pkg[0], 14*12) && cnt-->0){
		slogd("ch1_cali_coef_upper.txt read error\n");
		ret = -1;
	}
	cnt = 3;

	while(nvram_data_read(CH2_CALI_COEFF_FILE_LOWER, &param->dynamic_data.cali_coeff_pkg[1], 15*12) && cnt-->0){
		slogd("ch2_cali_coef_lower.txt read error\n");
		ret = -1;
	}
	cnt = 3;

	while(nvram_temp_read(TEMPERATURE_INIT_FILE, param->static_data.temperature_init, 2) && cnt-->0){
		slogd("cali_temperature.txt read error\n");
		ret = -1;
	}

	return ret;
}




