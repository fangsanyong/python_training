#include <iostream>
#include "opencv2/opencv.hpp"
#include "detection.h"

//#define YUVI420

int main(int argc, char **argv) 
{

    char displayEnv[] = "DISPLAY=192.168.1.186:0";
    putenv(displayEnv);

    char showresult[10];

    std::cout<<"\n_______________________detect___________________\n";


    // ///////////////////////////////////////////////////////////////////YUVI420//////////////////////////////////////////////////
    // /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     cv::Mat inimage = cv::imread("bus.jpg"); 
    ////////////////////BGR///////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef YUVI420
    OFFSCREEN pImgInfo;
    pImgInfo.i32Width = 810;
    pImgInfo.i32Height = 1080;
    pImgInfo.ppu8Plane[0] = (uchar *)malloc(pImgInfo.i32Width*pImgInfo.i32Height * 3 / 2);//Y首地址
    pImgInfo.ppu8Plane[1] = pImgInfo.ppu8Plane[0] + (pImgInfo.i32Width)*(pImgInfo.i32Height);//U的首地址
    pImgInfo.ppu8Plane[2] = pImgInfo.ppu8Plane[0] + ((pImgInfo.i32Width)*(pImgInfo.i32Height) * 5 / 4);//V的首地址
    pImgInfo.ppu8Plane[3] = 0;
    pImgInfo.pi32Pitch[0] = pImgInfo.i32Width;//Y在内存中的布长
    pImgInfo.pi32Pitch[1] = pImgInfo.i32Width / 2;//U在内存中的布长
    pImgInfo.pi32Pitch[2] = pImgInfo.i32Width / 2;//V在内存中的布长
    pImgInfo.pi32Pitch[3] = 0;
    pImgInfo.u32PixelArrayFormat = PAF_I420;
    FILE* file = fopen("output.yuv", "rb");
    if (!file) 
    {
        printf("无法打开YUV文件\n");
        return -1;
    }
    fread(pImgInfo.ppu8Plane[0], sizeof(unsigned char), (pImgInfo.i32Width)*(pImgInfo.i32Height)*3/2, file);
    fclose(file);
#else

   OFFSCREEN pImgInfo1;
   pImgInfo1.i32Width = inimage.cols;
   pImgInfo1.i32Height = inimage.rows;
   pImgInfo1.ppu8Plane[0] = (uchar *)malloc(pImgInfo1.i32Width*pImgInfo1.i32Height * 3 );//(unsigned char*)inimage.data;
   pImgInfo1.ppu8Plane[1] = 0;
   pImgInfo1.ppu8Plane[2] = 0;
   pImgInfo1.ppu8Plane[3] = 0;
   pImgInfo1.pi32Pitch[0] = pImgInfo1.i32Width*3;
   pImgInfo1.pi32Pitch[1] = 0;
   pImgInfo1.pi32Pitch[2] = 0;
   pImgInfo1.pi32Pitch[3] = 0;
   pImgInfo1.u32PixelArrayFormat =PAF_RGB24_B8G8R8;
  memcpy(pImgInfo1.ppu8Plane[0],inimage.data,pImgInfo1.i32Width*pImgInfo1.i32Height * 3);


//  cv::Mat input_img(320, 224, CV_8UC3);
//   cv::resize(inimage, input_img, cv::Size(320, 224));
//    OFFSCREEN pImgInfo1;
//    pImgInfo1.i32Width = input_img.cols;
//    pImgInfo1.i32Height = input_img.rows;
//    pImgInfo1.ppu8Plane[0] = (uchar *)malloc(pImgInfo1.i32Width*pImgInfo1.i32Height * 3 );//(unsigned char*)inimage.data;
//    pImgInfo1.ppu8Plane[1] = 0;
//    pImgInfo1.ppu8Plane[2] = 0;
//    pImgInfo1.ppu8Plane[3] = 0;
//    pImgInfo1.pi32Pitch[0] = pImgInfo1.i32Width*3;
//    pImgInfo1.pi32Pitch[1] = 0;
//    pImgInfo1.pi32Pitch[2] = 0;
//    pImgInfo1.pi32Pitch[3] = 0;
//    pImgInfo1.u32PixelArrayFormat =PAF_RGB24_B8G8R8;
//   memcpy(pImgInfo1.ppu8Plane[0],input_img.data,pImgInfo1.i32Width*pImgInfo1.i32Height * 3);
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    
    ZYOD_ENGINE ODEngine=NULL;
    int32_t res=-1;
    BoxRect result[MAXFACE] = { 0 };
    const char *model_path="pdcar-20230508.om";
    res = init(NULL, &ODEngine,model_path);

#ifdef YUVI420
    res = detection(NULL,ODEngine,&pImgInfo,result);
#else
    res = detection(NULL,ODEngine,&pImgInfo1,result);
#endif
    for(int i=0; i<result[0].box_number; i++)
    {
        printf("box_out[%d]: %6f,%6f,%6f,%6f,%6f,%d\n", i, result[i].xmin, result[i].ymin,result[i].xmax, result[i].ymax , result[i].score, result[i].classid);
    }

    for(int i=0;i<result[0].box_number;i++)
    {
        sprintf(showresult, "%d,%f",result[i].classid,result[i].score);
        // 在图像上绘制字符
        cv::putText(inimage, showresult, cv::Point(result[i].xmin+10, result[i].ymin+10), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);
        // 绘制矩形框
        cv::Rect rect(result[i].xmin, result[i].ymin, result[i].xmax - result[i].xmin, result[i].ymax - result[i].ymin);
        cv::rectangle(inimage, rect, cv::Scalar(0, 0, 255), 2);  // 绘制红色的矩形框，线条宽度为2

    }
    cv::imwrite("res.jpg", inimage);
    //cv::imshow("Image", oriimage);
    // 等待按下任意按键退出程序
   // cv::waitKey(0);
    // 关闭窗口
    //cv::destroyAllWindows();
    //ret.clear();

#ifdef YUVI420 
    if(pImgInfo.ppu8Plane[0])
    {
        free(pImgInfo.ppu8Plane[0]);
    }
#else
    if(pImgInfo1.ppu8Plane[0])
    {
        free(pImgInfo1.ppu8Plane[0]);
    }
#endif

    inimage.release();

    
    std::cout<<"\n_____________________end_____________________\n";
    
    res = uninit(NULL, &ODEngine);
    if (res != 0) 
    {
        printf("sample uninit resource failed\n");
        return -1;
    }

     if(ODEngine)
    {
        ODEngine=NULL;
    }

    return 0;
}