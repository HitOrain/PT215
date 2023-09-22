/*************************************************
* NDT MAIN LOOP
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#include "ndt_algorithm_process.h"

#include <semaphore.h>
#include <poll.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern NDT_ALGO_DATA st_ndt_algo_param;
 
sem_t sem;   
struct pollfd fds;
sigset_t mask;

int main(void)
{
	if(node_open(NODE, &st_ndt_algo_param.fd_ndt) < 0)
		goto NODE_NOT_EXIST;
	
	if(node_open(MOTOR, &st_ndt_algo_param.fd_motor) < 0)
		goto MOTOR_NOT_EXIST;
	
	if(driver_ready_detect() < 0)
		goto NODE_NOT_READY;

	fds.fd = st_ndt_algo_param.fd_ndt;
	fds.events = POLLIN;
	sigemptyset(&mask);

	ndt_init(&st_ndt_algo_param);

	data_init(&st_ndt_algo_param);

	aw8686x_dac_get(st_ndt_algo_param.ic_operation.dac);

	while(1)
	{

		ppoll(&fds, 1, NULL, &mask);

		if (fds.revents & POLLIN) {

			do{
				aw8686x_package_get(&st_ndt_algo_param.package, st_ndt_algo_param.all_offset_cancel_finish);

				screen_on(st_ndt_algo_param.package.state_pkg.screen_status);
				
				ndt_algo_param_update(&st_ndt_algo_param);

				ndt_offset_cancel(&st_ndt_algo_param.all_offset_cancel_finish, st_ndt_algo_param.ic_operation.dac);

				aw8686x_dac_set(st_ndt_algo_param.ic_operation.dac, st_ndt_algo_param.all_offset_cancel_finish);

				aw8686x_timer_controller(st_ndt_algo_param.all_offset_cancel_finish, &st_ndt_algo_param.timer_ctl);

			}while(FALSE==st_ndt_algo_param.all_offset_cancel_finish);

			ndt_algo_proc(&st_ndt_algo_param);

			aw8686x_event_write(st_ndt_algo_param.dynamic_data.button_state, st_ndt_algo_param.package.state_pkg.user_data.game_mode);

			interaction_interface(&st_ndt_algo_param);

		}
	}
	return 0;

NODE_NOT_READY:
	
	close(st_ndt_algo_param.fd_motor);

MOTOR_NOT_EXIST:
	
	close(st_ndt_algo_param.fd_ndt);

NODE_NOT_EXIST:
	
	return -1;
}




