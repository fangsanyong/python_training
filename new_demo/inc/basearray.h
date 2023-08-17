#ifndef __BASE_ARRAY_H__
#define __BASE_ARRAY_H__

#include "comdef.h"

typedef enum QAF_MAT_TYPE_CODE
{
	QAF_MAT_TYPE_CODE_8U = 0x1,
	QAF_MAT_TYPE_CODE_16S = 0x2,
	QAF_MAT_TYPE_CODE_32S = 0x3,
	QAF_MAT_TYPE_CODE_32F = 0x4
} QAF_MAT_TYPE_CODE;

typedef struct QAF_MAT_BASE
{
	MInt32 type;
	MInt32 rows;
	MInt32 cols;
	MInt32 channels;
	MInt32 linebytes;
	MUInt8 * data;
} QAF_MAT_BASE;


#define QAF_MAT_ELEM_8U(mat,row,col) (((mat)->data+row*(mat)->linebytes)[col])

#define QAF_MAT_ELEM_8U_C(mat,channel,row,col) (((mat)->data+row*(mat)->linebytes)[(col)*(mat)->channels+channel])


#define QAF_MAT_ELEM_16S(mat,row,col) (((MInt16*)(((mat)->data)+row*(mat)->linebytes))[col])

#define QAF_MAT_ELEM_16S_C(mat,channel,row,col) (((MInt16*)(((mat))->data+row*(mat)->linebytes))[(col)*(mat)->channels+channel])


#define QAF_MAT_ELEM_32S(mat,row,col) (((MInt32*)(mat)->data)[(row)*(mat)->cols+(col)])

#define QAF_MAT_ELEM_32S_C(mat,channel,row,col) (((MInt32*)(mat)->data)[(row)*(mat)->cols+(col)*(mat)->channels+channel])


#define QAF_MAT_ELEM_32F(mat,row,col) (((MFloat*)(mat)->data)[(row)*(mat)->cols+(col)])

#define QAF_MAT_ELEM_32F_C(mat,channel,row,col) (((MFloat*)(mat)->data)[(row)*(mat)->cols+(col)*(mat)->channels+channel])


#ifdef __cplusplus
extern "C" {
#endif


QAF_MAT_BASE * QAF_MAT_CreateHeader(MInt32 rows, MInt32 cols, MInt32 channels, MInt32 linebytes, MUInt8 * pData, 
								QAF_MAT_TYPE_CODE type, MHandle hMemMgr);

QAF_MAT_BASE * QAF_MAT_Create(MInt32 rows, MInt32 cols, MInt32 channels, QAF_MAT_TYPE_CODE type, MHandle hMemMgr);

MVoid QAF_MAT_Release(QAF_MAT_BASE ** mat, MHandle hMemMgr);

MVoid QAF_MAT_Print(const QAF_MAT_BASE * mat, const char * info);


#ifdef __cplusplus
}
#endif

#endif//__BASE_ARRAY_H__