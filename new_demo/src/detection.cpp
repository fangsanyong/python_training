#include "detection.h"
#include <string.h>
#include <algorithm>
#include "base.h"
#include "acl_options.h"
#include"sizecolorrotate.h"
#include "basearray.h"

#include "opencv2/opencv.hpp"

typedef struct ZYOD_RealEngine
{
    MHandle hMemMgr;
    const char *model_path;
    float confidence_threshold;
    float nms_threshold;
    int num_class;
    int nc;
    int modelinput_w,modelinput_h,modelinput_c,modelout_len; //模型尺寸
    QAF_MAT_BASE * qmatColorDetect;
    BlobStruct * net_input_blob;
   
    LPZY_BoxRect DetectionRes;
    SampleProcess* sampleProcess;

} ZYOD_RealEngine, *LPZYOD_RealEngine;

//get class

/**
 * @brief 获取输出中的最大值索引
*/
int getfclassid(std::vector<float> vec){
    int start_index = 5; // 起始索引
    int end_index = vec.size(); // 结束索引（不包含）
    std::vector<float>::iterator first = vec.begin() + start_index;
    std::vector<float>::iterator last = vec.begin() + end_index;
    std::vector<float> sub_vec(first, last);
    
    std::vector<float>::iterator max_it = std::max_element(sub_vec.begin(), sub_vec.end());
    int max_index = std::distance(sub_vec.begin(), max_it);

    return max_index;
}


///////////////////////////////////////////////////////////////////
//nms
float iou(const BoundingBox& a, const BoundingBox& b) {
    const float area_a = a.area();
    const float area_b = b.area();

    // 计算重叠区域的坐标范围
    const float x1 = std::max(a.x, b.x);
    const float y1 = std::max(a.y, b.y);
    const float x2 = std::min(a.x + a.w, b.x + b.w);
    const float y2 = std::min(a.y + a.h, b.y + b.h);

    // 计算重叠区域的面积
    const float intersection_area = std::max(0.0f, x2 - x1) * std::max(0.0f, y2 - y1);

    // 计算并集区域的面积
    const float union_area = area_a + area_b - intersection_area;

    // 计算IoU
    return union_area > 0 ? intersection_area / union_area : 0;
}

/**
 * @brief 计算nms 
*/
std::vector<BoundingBox> nms(std::vector<BoundingBox>& boxes, float threshold) {
    // 根据置信度排序
    std::sort(boxes.begin(), boxes.end(),
              [](const BoundingBox& a, const BoundingBox& b) { return a.confidence > b.confidence; });

    std::vector<BoundingBox> result;
    for (size_t i = 0; i < boxes.size(); ++i) {
        bool keep = true;
        for (size_t j = 0; j < result.size(); ++j) {
            if (iou(boxes[i], result[j]) > threshold) {
                keep = false;
                break;
            }
        }
        if (keep) {
            result.push_back(boxes[i]);
        }
    }
    return result;
}


int init(MHandle hMemMgr, ZYOD_ENGINE * pEngine,const char *model_path)
{   

    if (!pEngine || (*pEngine)||(!model_path))
	{
		return -1;
	}
    LPZYOD_RealEngine pRealEngine = (LPZYOD_RealEngine)malloc(sizeof(ZYOD_RealEngine));
	pRealEngine->hMemMgr = hMemMgr;


    // model info
    pRealEngine->num_class=CLASS_NUM;
    pRealEngine->nc=CLASS_NUM+5;
    pRealEngine->modelinput_w=MODEL_INPUT_W;//w;
    pRealEngine->modelinput_h=MODEL_INPUT_H;//h;
    pRealEngine->modelinput_c=MODEL_INPUT_C;//h;
    pRealEngine->modelout_len =MODEL_OUTHEAD_COUNT;//mo;
    pRealEngine->confidence_threshold=CONFIDENCE_TH;
    pRealEngine->nms_threshold=NMS_TH;

    pRealEngine->DetectionRes = (LPZY_BoxRect)malloc(sizeof(BoxRect)*MAXFACE);
	memset(pRealEngine->DetectionRes, 0, sizeof(BoxRect)*MAXFACE);
	pRealEngine->qmatColorDetect = QAF_MAT_Create(pRealEngine->modelinput_h, pRealEngine->modelinput_w,pRealEngine->modelinput_c, QAF_MAT_TYPE_CODE_8U, hMemMgr);


    pRealEngine->net_input_blob = (BlobStruct*)malloc(sizeof(BlobStruct));
	memset(pRealEngine->net_input_blob, 0, sizeof(BlobStruct));
	pRealEngine->net_input_blob->channel = pRealEngine->modelinput_c;
	pRealEngine->net_input_blob->width =  pRealEngine->modelinput_w;
	pRealEngine->net_input_blob->height =  pRealEngine->modelinput_h;
	pRealEngine->net_input_blob->num = pRealEngine->net_input_blob->channel*pRealEngine->net_input_blob->height*pRealEngine->net_input_blob->width;
	pRealEngine->net_input_blob->data_ptr = (DATATYPE*)malloc(sizeof(DATATYPE)*pRealEngine->net_input_blob->num);
	memset(pRealEngine->net_input_blob->data_ptr, 0, sizeof(DATATYPE)*pRealEngine->net_input_blob->num);

    // acl init 
    DEBUG_LOG("acl init");
    pRealEngine->sampleProcess = new SampleProcess(model_path);
    Result ret = pRealEngine->sampleProcess->InitResource();
    if (ret != SUCCESS) {
        ERROR_LOG("sample init resource failed");
        //_is_init = false;
        return -1;
    }
    INFO_LOG("acl init success");

     *pEngine = pRealEngine;
    return 0;
}


int detection(MHandle hMemMgr, ZYOD_ENGINE  Engine,LPOFFSCREEN pImgInfo,LPZY_BoxRect result_box)
{

    if (!Engine )//||// !pDetectionRes)//!hMemMgr || 
	{
		return -1;
	}
    LPZYOD_RealEngine pRealEngine =(LPZYOD_RealEngine)Engine;
    qaf_normalizeface_fromface(pImgInfo,pRealEngine->qmatColorDetect->data, pRealEngine->modelinput_w, pRealEngine->modelinput_h, pRealEngine->modelinput_w*pRealEngine->modelinput_c,pRealEngine->modelinput_c);

   // cv::Mat input_img(320, 224, CV_8UC3);
   // cv::resize(inimage, input_img, cv::Size(320, 224));


    
    // cv::Mat input_img(pRealEngine->modelinput_h, pRealEngine->modelinput_w, CV_8UC3);
    // memcpy(input_img.data,pRealEngine->qmatColorDetect->data,pRealEngine->modelinput_w*pRealEngine->modelinput_h*3);
    // cv::imwrite("bus_resize_bgr_fsy1.jpg", input_img);

    for (int c = 0; c < 3; c++)
	{
		for (int i = 0; i < pRealEngine->modelinput_h; i++)
		{
			for (int j = 0; j <pRealEngine->modelinput_w; j++)
			{
				pRealEngine->net_input_blob->data_ptr[pRealEngine->modelinput_w*pRealEngine->modelinput_h*c + pRealEngine->modelinput_w*i + j] = (pRealEngine->qmatColorDetect->data[i * pRealEngine->modelinput_w*3+ j * 3 +2- c]-0.0)/255.0;
			}
		}
	}
    Result ret = pRealEngine->sampleProcess->MainProcess(pRealEngine->net_input_blob->data_ptr);
    if (ret != SUCCESS) 
    {
        ERROR_LOG("MainProcess failed");
        return -1;
    }
    INFO_LOG("MainProcess success");
    auto outputsf = pRealEngine->sampleProcess->outData;
    //postprogress

    std::vector<float> outputvtemp; 
    std::vector<std::vector<BoundingBox>> preds(pRealEngine->num_class); //class-bboxes
    BoundingBox temp;
    auto n = pRealEngine->modelout_len*pRealEngine->nc;
    // int c=0;
    for(auto i=0;i<n;i++){
        outputvtemp.push_back(outputsf[i]);
        // INFO_LOG("----%d",c);
        // c+=1;
        if((i+1)%pRealEngine->nc==0) {
            if(outputvtemp[4]>pRealEngine->confidence_threshold){
                auto cx = outputvtemp[0];
                auto cy = outputvtemp[1];
                auto w = outputvtemp[2];
                auto h = outputvtemp[3];
                // std::cout<<"\ncxcywhs "<<cx<<' '<<cy<<' '<<w<<' '<<h<<' '<<outputvtemp[4];
;                //left-up image-wh
                temp.x = std::max(int((cx-w/2)*pImgInfo->i32Width),0);
                temp.y = std::max(int((cy-h/2)*pImgInfo->i32Height),0);
                temp.w = std::min(int(w*pImgInfo->i32Width),int(pImgInfo->i32Width-temp.x));
                temp.h = std::min(int(h*pImgInfo->i32Height),int(pImgInfo->i32Height-temp.y));
                // temp.x = int((cx-w/2)*inputw);
                // temp.y = int((cy-h/2)*inputh);
                // temp.w = int(w*inputw);
                // temp.h = int(h*inputh);
                temp.cx = int(cx*pImgInfo->i32Width);
                temp.cy = int(cy*pImgInfo->i32Height);
                temp.confidence =  outputvtemp[4];
                temp.classid = getfclassid(outputvtemp);

                preds[temp.classid].push_back(temp);
                //std::cout<<std::endl<<temp.x<<' '<<temp.y<<' '<<temp.w<<' '<<temp.h<<' '<<temp.classid<<'\n';
            }
            outputvtemp.clear();
        } 
    }
    //nms
    pRealEngine->DetectionRes[0].box_number=0;
    int count=0;
    for(int i=0;i<pRealEngine->num_class;i++)
    { //TODO: 与nc合并
        auto result_singleclass = nms(preds[i],pRealEngine->nms_threshold);
        for(int j=0;j<result_singleclass.size();j++)
        {
           pRealEngine->DetectionRes[count].xmin=std::max((float)0,result_singleclass[j].x);
           pRealEngine->DetectionRes[count].ymin=std::max((float)0,result_singleclass[j].y);
           pRealEngine->DetectionRes[count].xmax=std::min((float)pImgInfo->i32Width,result_singleclass[j].x+result_singleclass[j].w);
           pRealEngine->DetectionRes[count].ymax=std::min((float)pImgInfo->i32Height,result_singleclass[j].y+result_singleclass[j].h);
           pRealEngine->DetectionRes[count].classid=result_singleclass[j].classid;
           pRealEngine->DetectionRes[count].score=result_singleclass[j].confidence;
           count+=1;
        }
        pRealEngine->DetectionRes[0].box_number+=result_singleclass.size();

    }

     
    for(int id=0;id<pRealEngine->DetectionRes[0].box_number;id++)
    {
         result_box[id].xmin=pRealEngine->DetectionRes[id].xmin;//lx
         result_box[id].ymin=pRealEngine->DetectionRes[id].ymin;//ly
         result_box[id].xmax=pRealEngine->DetectionRes[id].xmax;//rx
         result_box[id].ymax=pRealEngine->DetectionRes[id].ymax;//ry
         result_box[id].score=pRealEngine->DetectionRes[id].score;//score
         result_box[id].classid=pRealEngine->DetectionRes[id].classid;//class
    }
    result_box[0].box_number=pRealEngine->DetectionRes[0].box_number;
    //input_img.release();
    return 0;
}


int uninit(MHandle hMemMgr, ZYOD_ENGINE * pEngine)
{
    if (!pEngine || !(*pEngine))
	{
		return -1;
	}
    LPZYOD_RealEngine pRealEngine = (LPZYOD_RealEngine)(*pEngine);

	if (pRealEngine->qmatColorDetect)
	{
		QAF_MAT_Release(&pRealEngine->qmatColorDetect, hMemMgr);
	}
	if (pRealEngine->net_input_blob->data_ptr)
	{
        free(pRealEngine->net_input_blob->data_ptr);
		pRealEngine->net_input_blob->data_ptr = 0;
	}

	if (pRealEngine->net_input_blob)
	{
		free(pRealEngine->net_input_blob);
		pRealEngine->net_input_blob = 0;
	}
//////////////////////////////////////////////////////////////////
	if (pRealEngine->DetectionRes)
	{
		free(pRealEngine->DetectionRes);
		pRealEngine->DetectionRes= 0;
	}
////////////////////////////////////////////////////////////////////free
     delete(pRealEngine->sampleProcess);
     if(pRealEngine)
     {
     free(pRealEngine);
	 pRealEngine = NULL;
     }
	*pEngine = NULL;
    return 0;
}
