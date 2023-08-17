#ifndef _DETECTION_H_
#define _DETECTION_H_

#include"offscreen.h"
#define  MAXFACE 50

#ifdef __cplusplus
extern "C" {
#endif

typedef void*	ZYOD_ENGINE;	
typedef void*	MHandle;

typedef struct
{
	float xmin;
	float ymin;
	float xmax;
	float ymax;
	float score;
	int classid;
	int box_number;
} BoxRect, *LPZY_BoxRect;

int init(MHandle hMemMgr, ZYOD_ENGINE * pEngine,const char *model_path);
int detection(MHandle hMemMgr, ZYOD_ENGINE  Engine,LPOFFSCREEN pImgInfo,LPZY_BoxRect result_box);
int uninit(MHandle hMemMgr, ZYOD_ENGINE * pEngine);
#ifdef __cplusplus
}
#endif

#endif // _DETECTION_H_