/**************************************************************
 * 检测接口
**************************************************************/


#ifndef BASE_H
#define BASE_H
typedef float DATATYPE;

typedef struct BlobStruct
{
	int channel;
	int height;
	int width;
	int num;
	DATATYPE * data_ptr;
} BlobStruct;


typedef struct BoundingBox {
    float x, y, w, h, cx, cy; //ltxy-wh-cxy
    float confidence;
    int classid;
    float area() const { return w * h; }
    int id=-1;
    int box_number;
} BoundingBox,*LPBoundingBox;



#endif //BASE_H