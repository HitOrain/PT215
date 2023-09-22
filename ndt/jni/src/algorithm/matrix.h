#ifndef __MATRIX__H__
#define __MATRIX__H__

#include "compile_option.h"


#define MATRIX_DIM 5
#define SIG_DIM CH_G1
#define MATRIX_ST_LEN (1 + MATRIX_DIM * (SIG_DIM + MATRIX_DIM + 3))

typedef struct
{

    s32 W1[MATRIX_DIM][SIG_DIM];
    s32 W2[MATRIX_DIM][MATRIX_DIM];
    s32 W3[MATRIX_DIM]; //1*5
    s32 B1[MATRIX_DIM]; //5*1
    s32 B2[MATRIX_DIM]; //5*1
    s32 B3;

} ST_OUT_LIER_MATRIX;

extern ST_OUT_LIER_MATRIX st_matrix_para;



extern s16 calc_checksum_s16(u8 * buff, u16 len);
extern void init_outlier_para(void);
extern void sort(s16 *Sorce, u8 *Sequence, u8 Len, u16 step );
extern u16 calc_u16_sqrt(u32 input);
extern u8 bin_2_dec(u16 input);
u8 calc_outlier_matrix(s32 *lspSig);

#endif
