/*************************************************
* DEFINE For CUSTOMERS' FUNCTION
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/


#ifndef __CUSTOMER_COMMON_H
#define __CUSTOMER_COMMON_H

#include "compile_option.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct{
	bool cur_bu_event;
	bool pre_bu_event;

	u16 scan_rate_high;
	u16 scan_rate_low;
	u16 scan_rate_deep_sleep;
	u16 scan_rate_poweroff;
} ST_CUS_PARA;

extern ST_CUS_PARA st_cus_para;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern void cus_cmd_detect(u8 cmd);
extern void cus_scan_mode_detect(void);
extern void cus_monitor(void);
extern void cus_handshake_detect(u8 handshake);
extern void cus_flash_para_reinit(void);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif /* __CUSTOMER_COMMON_H */

