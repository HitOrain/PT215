/****************************************************************
* NDT COMPILE OPTION
* Copyright(c) 2018, NewDegreeTechnology, Inc.
*****************************************************************/

#ifndef __COMPILE_OPTION_H
#define __COMPILE_OPTION_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PROJECT_ID/100;PROJECT_ID%100   //vivo 
//1：F61
//2：F70
//3：F71
//4：AW86802
//5：AW86862
//6：M68
//7：F62
//8：F72

#define NDT_MAIN_VER	42
#define NDT_SUB_VER	    15

#define FRAMEWORK_VER   1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Define for Customer */

#define VIVO_AW_8686X_PT215_C1V1_UG_V1 		1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Using for MCU */
#if (VIVO_AW_8686X_PT215_C1V1_UG_V1)
#define AW_8686X
#endif

#include "algorithm_common.h"

/////////////////////////////////////////////////////////////////////////////////////////

#if(VIVO_AW_8686X_PT215_C1V1_UG_V1)
#include "vivo_aw_8686x_pt215_c1v1_ug_v1.h"
#endif

#ifndef PHY_CH_COUNT
#define PHY_CH_COUNT    CH_COUNT
#endif

#if (CH_COUNT != PHY_CH_COUNT && !defined(USE_MIXED_RAW))
#define USE_MIXED_RAW
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Using for NDT Sensors */

#define ADC_ACCURACY_12BIT	1
#define ADC_ACCURACY_24BIT	0

#ifndef OFFSET_CANCEL_BASE_LINE
#if(ADC_ACCURACY_12BIT)
#define OFFSET_CANCEL_BASE_LINE		(2048)
#endif

#if(ADC_ACCURACY_24BIT)
#define OFFSET_CANCEL_BASE_LINE               (1 << 23)
#endif
#endif

#ifndef DAC_MIDDLE_VAL
#define DAC_MIDDLE_VAL (1024)
#endif // !DAC_MIDDLE_VAL

/////////////////////////////////////////////////////////////////////////////////////////

#if (ADC_ACCURACY_12BIT)

typedef int16_t AdcType;
typedef int16_t DacType;

#elif (ADC_ACCURACY_24BIT)

typedef int32_t AdcType;
typedef int32_t DacType;

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  /* __COMPILE_OPTION_H */

