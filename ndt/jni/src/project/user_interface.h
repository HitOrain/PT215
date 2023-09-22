/*************************************************
* NDT USER INTERFACE
* Copyright(c) 2018, NewDegreeTechnology, Inc.
**************************************************/

#ifndef __USER_INTERFACE_H
#define __USER_INTERFACE_H

#include <stdint.h>
#include <stdbool.h>

//////////////////////////////////////////////////////////////////////////////////////////

#define CMD_SW_RESET                    0x01

#define CMD_RESET_FORCE_FLAG            0x10
#define CMD_RESET_BASE_LINE             0x11
#define CMD_OFFSET_CANCEL               0x12
#define CMD_OFFSET_CANCEL_EXIT          0x13


#define CMD_IO_AUTO_TEST                0x30
#define CMD_GPIO_STATUS                 0x31

#define CMD_ENTER_CHIP_NOISE_MODE       0x40
#define CMD_EXIT_CHIP_NOISE_MODE        0x41

#define CMD_CUS_DEFINE_0				0x80
#define CMD_CUS_DEFINE_1				0x81
#define CMD_CUS_DEFINE_2				0x82
#define CMD_CUS_DEFINE_3				0x83
#define CMD_CUS_DEFINE_4				0x84
#define CMD_CUS_DEFINE_5				0x85


//////////////////////////////////////////////////////////////////////////////////////////

typedef uint8_t IIcType;



typedef struct{
	IIcType *data_ptr;
	bool control_flag;

	IIcType r_pointer;
	IIcType w_pointer;
	uint8_t r_count;
	uint8_t w_count;

} ST_IIC;

extern volatile ST_IIC st_iic;

//////////////////////////////////////////////////////////////////////////////////////////

#define MODE_COUNT	4

#define CMD_COUNT		1
#define RW_COUNT		1
#define RW_DATA_COUNT		255

typedef struct{
	IIcType cmd_buf[CMD_COUNT];
	IIcType rw_cound_buf[RW_COUNT];
	IIcType rw_data_buf[RW_DATA_COUNT]; // notice: here must start align 2
#if ((CMD_COUNT + RW_COUNT + RW_DATA_COUNT) % 2)
	IIcType align_byte;
#endif
} ST_INTERACTION;

extern volatile ST_INTERACTION st_interaction[MODE_COUNT];

//////////////////////////////////////////////////////////////////////////////////////////

extern void register_select(IIcType iic_data);
extern void interaction_interface(void);

//////////////////////////////////////////////////////////////////////////////////////////

#endif  /* __USER_INTERFACE_H */
