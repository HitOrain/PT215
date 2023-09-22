#ifndef __ALGORITHM_COMMON__H__
#define __ALGORITHM_COMMON__H__

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#define JOB_OK  1
#define JOB_ERR 0XEE

#define EVENT_BUSY 1
#define EVENT_IDLE 0

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

#define SQUARE(x)	((x) * (x))
#if !(defined(CS_F71) || defined(CS_F70))
    #define ABS(x)		(((x) > 0) ? (x) :(-(x)))
#endif
#define MAX(x, y)	((x) > (y) ? (x) : (y))
#define MIN(x, y)	((x) > (y) ? (y) : (x))
#define SUBABS(x, y) (((x) > (y)) ? ((x) - (y)) :((y) - (x)))
#define	ABSDATA(x, y) (((x) - (y) > 0) ? ((x) - (y)) : ((y) - (x)))
#define RELU(X) 	((X) > 0 ? (X) : (0))


#define MIN_S8 (0x80)
#define MAX_S8 (0x7f)

#define MIN_S16 (0x8000)
#define MAX_S16 (0x7fff)

#define MIN_S32 (0x80000000)
#define MAX_S32 (0x7fffffff)

#define MIN_U8 (0x00)
#define MAX_U8 (0xff)

#define MIN_U16 (0x0000)
#define MAX_U16 (0xffff)

#define MIN_U32 (0x00000000)
#define MAX_U32 (0xffffffff)


///////////////includes//////////////////////////
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#endif
