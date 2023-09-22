/*************************************************
* NDT TOUCH FLAG
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/
#ifndef __TOUCH_PARSE_H
#define __TOUCH_PARSE_H

#include "data_parse.h"
#include "algorithm_init.h"
#include "compile_option.h"
//#include <sys/types.h>
//#include <sys/stat.h>
#include <fcntl.h>
///#include <unistd.h>
//#include <sys/ioctl.h>
//#include <sys/mman.h>

/////////////////////////////////////////////////////////////////////////////////////////

int adc_sample();
void kernel_all_adc_dac_get(AdcType *adc_data, DacType *dac_data, uint8_t ch_count);
void hal_channel_check(uint8_t ch_count);

/////////////////////////////////////////////////////////////////////////////////////////

#endif /* __TOUCH_PARSE_H */

