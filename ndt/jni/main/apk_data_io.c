#include "ndt_algorithm_process.h"
#include <unistd.h>

static void sys_cmd_detect(NDT_ALGO_DATA* param)
{

	unsigned int length = 0;
	int timeout = 100;
	
	if(param->package.state_pkg.apk_command.apk_cmd != 0)
	{
		switch(param->package.state_pkg.apk_command.apk_cmd)
		{
			case FW_VERSION:
				
				length = sizeof(param->static_data.version_id);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.version_id), length);
				aw8686x_count_write(length);
				
		    break; 

	        case ADC_UV:
				
				length = sizeof(param->static_data.adc_conv_factor);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.adc_conv_factor), length);
				aw8686x_count_write(length);

	        break;

			case DAC_MV:
				
				length = sizeof(param->static_data.dac_conv_factor);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.dac_conv_factor), length);
				aw8686x_count_write(length);	
				
	        break;
				
			case AW_OFFSET:
				
				length = sizeof(param->static_data.offset);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.offset), length);
				aw8686x_count_write(length);
				
			break;

			case CALI_CHECK:
				
				length = sizeof(param->static_data.cali_coeff);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.cali_coeff), length);
				aw8686x_count_write(length);
				
			break;

			case AW_TEMP:
				
				length = sizeof(param->static_data.temperature_real);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.temperature_real), length);
				aw8686x_count_write(length);
				
			break;

			case AW_CALI_TEMP:
				
				length = sizeof(param->static_data.temperature_init);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->static_data.temperature_init), length);
				aw8686x_count_write(length);

			break;

			case FORCE_LOG:

				length =  sizeof(param->dynamic_data.force_log);
				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->dynamic_data.force_log), length);
				aw8686x_count_write(length);
				
			break;

			case CALI_LOG:
				length = cali_log_update((void*)(&param->dynamic_data.cali_log));
				timeout = 100;
				if(length <= sizeof(param->dynamic_data.cali_log)) {
					aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->dynamic_data.cali_log), length);
					aw8686x_count_write(length);
					do{
						aw8686x_count_read(&param->dynamic_data.apk_cnt);
						usleep(100);
					}while(param->dynamic_data.apk_cnt && timeout-- < 0);
						
				} else {
					slogd("selfcheck_log is not enough\n");
				}
				
			break;
			
			case CHECK_LOG:

				length = sizeof(param->dynamic_data.selfcheck_log);
				if(length <= sizeof(param->dynamic_data.selfcheck_log)) {
					aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->dynamic_data.selfcheck_log), length);
					aw8686x_count_write(length);
				} else {
					slogd("selfcheck_log is not enough\n");
				}

			break;

			case EFFECT_LOG:
				
				length = effect_log_update((void*)(&param->dynamic_data.effect_log));
				if(length <= sizeof(param->dynamic_data.effect_log)) {
					aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(&param->dynamic_data.effect_log), length);
					aw8686x_count_write(length);
					timeout = 100;
					do{
						aw8686x_count_read(&param->dynamic_data.apk_cnt);
						usleep(100);
					}while(param->dynamic_data.apk_cnt && timeout-- < 0);
				} else {
					slogd("effect_log is not enough\n");
				}

			break;

			case CALI_COEF:
			break;

			case SENSOR_STATUS:
				param->dynamic_data.button_state[1] = 0x03;
				aw8686x_data_write(FORCE_EVENT, (void*)(param->dynamic_data.button_state), sizeof(param->dynamic_data.button_state));

				aw8686x_data_write(SELF_CHECK, NULL, 0);
				while(!param->ic_operation.on_off_status_all[0]){
					aw8686x_data_read(SELF_CHECK, (void*)(param->ic_operation.on_off_status_all), sizeof(param->ic_operation.on_off_status_all));
				}

				on_off_status_update(param->dynamic_data.on_off_status, param->ic_operation.on_off_status_all);

				aw8686x_data_write(param->package.state_pkg.apk_command.apk_cmd, (void*)(param->dynamic_data.on_off_status), sizeof(param->dynamic_data.on_off_status));
				aw8686x_count_write(10);

				param->dynamic_data.button_state[1] = 0x00;
				aw8686x_data_write(FORCE_EVENT, (void*)(&param->dynamic_data.button_state), sizeof(param->dynamic_data.button_state));

			break;

			default:
				slogd("CMD error!\n");
			break;
		}
		
		aw8686x_cmd_write(0x00);
	}

}

inline void interaction_interface(NDT_ALGO_DATA* param)
{
	factory_detect(&param->package.state_pkg.apk_command.factory_mod);
	sys_cmd_detect(param);
	chip_noise_detect(&param->package.state_pkg.apk_command.apk_mod);
}

static void inter_mode_in(void)
{
	int timeout = 1000;
	unsigned char cmd = 1;
	unsigned char factory_mod = 0;
	
	aw8686x_data_write(INTER_FAULT_MODE, (void*)(&cmd), 1);
	do {
		if(aw8686x_data_read(INTER_MODE_STATE, (void*)(&factory_mod), sizeof(factory_mod))){
			factory_mod = 0;
		}
		usleep(10000);
	} while(factory_mod == 0 && timeout-- > 0);
	
	if(timeout <= 0)
	{
		slogd("factory_in timeout\n");
	}
}

static void inter_mode_out(void)
{
	int timeout = 10000;
	unsigned char cmd = 0;
	unsigned char factory_mod = 1;
	
	aw8686x_data_write(INTER_FAULT_MODE, (void*)(&cmd), 1);
	do {
		if(aw8686x_data_read(INTER_MODE_STATE, (void*)(&factory_mod), sizeof(factory_mod))){
			factory_mod = 1;
		}
		usleep(10000);
	} while(factory_mod == 1 && timeout-- > 0);
	
	if(timeout <= 0)
	{
		slogd("factory_out timeout\n");
	}
}

void factory_detect(unsigned char factory_mod[1])
{
	static int cnt = 0;
	
	if(*factory_mod && cnt++ != 0) {
		cnt = 0 ;
		*factory_mod = 0;
		aw8686x_data_write(FACTORY_MODE, (void*)(factory_mod), sizeof(factory_mod));
	}
}

void chip_noise_detect(unsigned char apk_mod[1])
{
	static int timeout=0;

	if(1==*apk_mod){
		inter_mode_in();
		timeout = 300000;
		*apk_mod = 2;
		aw8686x_data_write(APK_MOD, (void*)(apk_mod), sizeof(apk_mod));
	}

	if(2==*apk_mod){
		timeout--;
	}

	if(timeout < 0 || 3==*apk_mod){
		inter_mode_out();
		*apk_mod = 0;
		aw8686x_data_write(APK_MOD, (void*)(apk_mod), sizeof(apk_mod));
	}
}


