#include "matrix.h"

ST_OUT_LIER_MATRIX st_matrix_para;

//calc checksum16
s16 calc_checksum_s16(u8 * buff, u16 len)
{
    s16 sum = 0;

    if (len % 2)
    {
        return -1;
    }

    while(len > 0)
    {
        sum += *buff++;
        sum += (u16)(*buff++) << 8;

        len -= 2;
    }

    return sum;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//通用排序函数,跳跃n个步进求
void sort(s16 *Sorce, u8 *Sequence, u8 Len, u16 step)
{
	int i;
	int j;
	int temp;
    for (i = 0; i < Len; i++)
    {
        Sequence[i] = i;  //遍历给索引数组赋值为原数组的下标
    }

    for(i = 1; i < Len ; i++)
    {
        j = i;
        temp = Sequence[j];

        while (j > 0 && *(Sorce + temp * step) > *(Sorce + Sequence[j - 1] * step))
        {
            Sequence[j] = Sequence[j - 1];
            j--;
        }

        Sequence[j] = temp;
    }
}


u16 calc_u16_sqrt(u32 input)
{
    u32 data_x = (MAX_U16 >> 1) + 1;
    u32 data_d = (MAX_U16 >> 2) + 1;

    while(data_d)
    {
        if (data_x * data_x > input)
        {
            data_x -= data_d;
        }
        else if (data_x * data_x < input)
        {
            data_x += data_d;
        }
        else
        {
            data_d = 0;
        }

        data_d >>= 1;
    }

    return (u16)data_x;
}

u8 bin_2_dec(u16 input)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        if ((1 << i) == input)
        {
            return i;
        }
    }

    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u8 lubRowNumA;
u8 lubColNumA;
u8 lubRowNumB;
u8 lubColNumB;

/*****************************************/
//function : 矩阵点乘运算
//input    : 矩阵A,B,参数指针（表示AB的行列）
//output   : 矩阵C = A.*B
/*****************************************/
u8 matrix_point_mul(s32 *lspMatrixA, s32 *lspMatrixB, s32 *lspMatixC, u8 *lubRowColAB)
{
    u8 i, j, k;
    s32 luwSum = 0;

    lubRowNumA = *lubRowColAB;
    lubColNumA = *(lubRowColAB + 1);
    lubRowNumB = *(lubRowColAB + 2);
    lubColNumB = *(lubRowColAB + 3);

    if (lubColNumA != lubRowNumB)
    {
        return false;
    }

    for (i = 0; i < lubRowNumA; i++)
    {
        for (j = 0; j < lubColNumB; j++)
        {

            luwSum = 0;

            for (k = 0; k < lubColNumA; k++)
            {
                luwSum += *(lspMatrixA + (u16)i * lubColNumA + k) * *(lspMatrixB + j + (u16)k * lubColNumB);
            }

            *(lspMatixC + (u16)i * lubColNumB + j) = luwSum;
        }
    }

    return true;
}

/*****************************************/
//function : 矩阵加法运算
//input    : 矩阵A,B,参数指针（表示AB的行列）
//output   : 矩阵C = A+B
/*****************************************/
u8 matrix_add(s32 *lspMatrixA, s32 *lspMatrixB, s32 *lspMatixC, u8 *lubRowColAB)
{
    u8 i, j;
    s16 luwIndex;

    lubRowNumA = *lubRowColAB;
    lubColNumA = *(lubRowColAB + 1);
    lubRowNumB = *(lubRowColAB + 2);
    lubColNumB = *(lubRowColAB + 3);

    if (lubRowNumA != lubRowNumB
            || lubColNumA != lubColNumB
       )
    {
        return false;
    }

    for (i = 0; i < lubRowNumA; i++)
    {
        for (j = 0; j < lubColNumA; j++)
        {
            luwIndex = (u16)i * lubColNumA + j;
            *(lspMatixC + luwIndex) = *(lspMatrixA + luwIndex) + *(lspMatrixB + luwIndex);
        }
    }

    return true;

}

/*****************************************/
//function : 矩阵映射1
//input    : 矩阵A，参数指针（表示AB的行列）
//output   : 矩阵A = relu(A)
/*****************************************/
void matrix_mapping(s32 *lspMatrixA, u8 *lubRowColA)
{
    lubRowNumA = *lubRowColA;
    lubColNumA = *(lubRowColA + 1);

    u16 luwSizeMatrix = (u16)lubRowNumA * lubColNumA;
    s32 *lspMatrix = lspMatrixA;

    while (luwSizeMatrix--)
    {
        *lspMatrix = RELU(*lspMatrix);
        lspMatrix++;
    }
}

s32 lspTemp1[MATRIX_DIM]; //临时矩阵1
s32 lspTemp2[MATRIX_DIM]; //临时矩阵2
u8 lubMatixP[4]; //矩阵的row col

/*****************************************/
//function : 计算误触结果
//input    : forcesig指针
//output   : 0:err; 1:ok
/*****************************************/
u8 calc_outlier_matrix(s32 *lspSig)
{
    u8 i;

    lubMatixP[0] = MATRIX_DIM;
    lubMatixP[1] = SIG_DIM;
    lubMatixP[2] = SIG_DIM;
    lubMatixP[3] = 1;
    matrix_point_mul(&st_matrix_para.W1[0][0], lspSig, lspTemp1, lubMatixP);
    lubMatixP[0] = MATRIX_DIM;
    lubMatixP[1] = 1;
    lubMatixP[2] = MATRIX_DIM;
    lubMatixP[3] = 1;
    matrix_add(lspTemp1, &st_matrix_para.B1[0], lspTemp2, lubMatixP);

    for (i = 0; i < MATRIX_DIM; i++)
    {
        lspTemp2[i] >>= 10;
    }

    matrix_mapping(lspTemp2, lubMatixP);

    lubMatixP[0] = MATRIX_DIM;
    lubMatixP[1] = MATRIX_DIM;
    lubMatixP[2] = MATRIX_DIM;
    lubMatixP[3] = 1;
    matrix_point_mul(&st_matrix_para.W2[0][0], lspTemp2, lspTemp1, lubMatixP);
    lubMatixP[0] = MATRIX_DIM;
    lubMatixP[1] = 1;
    lubMatixP[2] = MATRIX_DIM;
    lubMatixP[3] = 1;
    matrix_add(lspTemp1, &st_matrix_para.B2[0], lspTemp2, lubMatixP);

    for (i = 0; i < MATRIX_DIM; i++)
    {
        lspTemp2[i] >>= 10;
    }

    matrix_mapping(lspTemp2, lubMatixP);


    lubMatixP[0] = 1;
    lubMatixP[1] = MATRIX_DIM;
    lubMatixP[2] = MATRIX_DIM;
    lubMatixP[3] = 1;
    matrix_point_mul(&st_matrix_para.W3[0], lspTemp2, lspTemp1, lubMatixP);
    lubMatixP[0] = 1;
    lubMatixP[1] = 1;
    lubMatixP[2] = 1;
    lubMatixP[3] = 1;
    matrix_add(lspTemp1, &st_matrix_para.B3, lspTemp2, lubMatixP);

    // z > ln(p/1-p)
    if (*lspTemp2 > 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

const s32 memInit[] =
{
    -418, 696, -229, 25, 161, 1721, -442, 750, -305, -108, 1484, -943, -698, 266, -735, -462, 1394, 1376, 796, -1324, -387, 852, -96, 92, -163, -787, -628, -787, -521, 44, 1044, 1143, -1156, 955, 875, 195, 239, -246, 187, 163, 498, 566, -578, 465, 418, -1195, -1262, 1688, -1413, -774, 778, 846, -866, 706, 643, -2377, -517, -1191, 3469, -1779, -289427, -491830, -242863, -780022, 441669, -49761, -38453, -45587, 1678043, -47451, 375057,
};

void init_outlier_para(void)
{
//    memcpy(&st_matrix_para.W1[0][0], memInit, sizeof(memInit));
}

