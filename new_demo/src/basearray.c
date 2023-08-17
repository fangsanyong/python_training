#include "basearray.h"
#include "stdio.h"
#include <stdlib.h>
#include <string.h>


#undef QAF_ALIGN
#define QAF_ALIGN(size) (((size)+3) & -4)


QAF_MAT_BASE * QAF_MAT_CreateHeader(MInt32 rows, MInt32 cols, MInt32 channels, MInt32 linebytes, MUInt8 * pData, QAF_MAT_TYPE_CODE type, MHandle hMemMgr)
{
	QAF_MAT_BASE * base_mat = 0;
	MInt32 totalsize = sizeof(QAF_MAT_BASE);

	base_mat = (QAF_MAT_BASE *)malloc(totalsize);
	if (!base_mat)
		return 0;
	memset(base_mat, 0, totalsize);

	base_mat->type = type;
	base_mat->rows = rows;
	base_mat->cols = cols;
	base_mat->channels = channels;
	base_mat->linebytes = linebytes;
	base_mat->data = pData;

	return base_mat;
}

QAF_MAT_BASE * QAF_MAT_Create(MInt32 rows, MInt32 cols, MInt32 channels, QAF_MAT_TYPE_CODE type, MHandle hMemMgr)
{
	QAF_MAT_BASE * base_mat = 0;
	MInt32 linebytes = 0;
	MInt32 totalsize = 0;
	
	switch (type)
	{
	case QAF_MAT_TYPE_CODE_8U:
		linebytes = QAF_ALIGN(cols*channels*sizeof(MUInt8));
		break;
	case QAF_MAT_TYPE_CODE_16S:
		linebytes = QAF_ALIGN(cols*channels*sizeof(MInt16));
		break;
	case QAF_MAT_TYPE_CODE_32S:
		linebytes = QAF_ALIGN(cols*channels*sizeof(MInt32));
		break;
	case QAF_MAT_TYPE_CODE_32F:
		linebytes = QAF_ALIGN(cols*channels*sizeof(MFloat));
		break;
	default:
		return 0;
		break;
	}
	
	totalsize = rows*linebytes + sizeof(QAF_MAT_BASE);
	
	base_mat = (QAF_MAT_BASE *)malloc(totalsize);
	if (!base_mat)
		return 0;
	memset(base_mat, 0, totalsize);
	
	base_mat->type = type;
	base_mat->rows = rows;
	base_mat->cols = cols;
	base_mat->channels = channels;
	base_mat->linebytes = linebytes;
	base_mat->data = (MUInt8 *)(base_mat+1);

	return base_mat;
}

MVoid QAF_MAT_Release(QAF_MAT_BASE ** mat, MHandle hMemMgr)
{
	if (mat && *mat)
	{
		//free(hMemMgr, (MVoid*)(*mat));
		//free((MVoid*)(*mat));
		free(*mat);
		*mat = 0;
	}
}

MVoid QAF_MAT_Print(const QAF_MAT_BASE * mat, const char * info)
{
	MInt32 i, j, k;
	
	printf("%s:\n", info);
	
	for (i = 0; i < mat->rows; i++)
	{
		for (j = 0; j < mat->cols; j++)
		{
			for (k = 0; k < mat->channels; k++)
			{
				switch (mat->type)
				{
				case QAF_MAT_TYPE_CODE_8U:
					printf("%d\t", QAF_MAT_ELEM_8U_C(mat, k, i, j));
					break;
				case QAF_MAT_TYPE_CODE_16S:
					printf("%d\t", QAF_MAT_ELEM_16S_C(mat, k, i, j));
					break;
				case QAF_MAT_TYPE_CODE_32S:
					printf("%d\t", QAF_MAT_ELEM_32S_C(mat, k, i, j));
					break;
				case QAF_MAT_TYPE_CODE_32F:
					printf("%f\t", QAF_MAT_ELEM_32F_C(mat, k, i, j));
					break;
				default:
					break;
				}
			}
		}
		printf("\n");
	}
}

