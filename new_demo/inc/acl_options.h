#ifndef ACL_OPTIONS_H
#define ACL_OPTIONS_H

#include <map>
#include <iostream>
#include <memory>
#include <fstream>
#include <unistd.h>
#include <string>
#include <dirent.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "acl/acl.h"


#define INFO_LOG(fmt, ...) fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) fprintf(stdout, "[WARN]  " fmt "\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...)fprintf(stderr, "[ERROR]  " fmt "\n", ##__VA_ARGS__)
#define DEBUG_LOG(fmt, ...)fprintf(stdout, "[DEBUG]  " fmt "\n", ##__VA_ARGS__)

////////////////////////模型尺寸
#define MODEL_INPUT_W 320
#define MODEL_INPUT_H 224
#define MODEL_INPUT_C 3
#define MODEL_OUTHEAD_COUNT 4410
#define CLASS_NUM 4
#define CONFIDENCE_TH 0.5
#define NMS_TH 0.5



typedef enum Result {
    SUCCESS = 0,
    FAILED = 1
} Result;



template<class Type>
std::shared_ptr<Type> MakeSharedNoThrow()
{
    try {
        return std::make_shared<Type>();
    } catch (...) {
        return nullptr;
    }
}

#define MAKE_SHARED_NO_THROW(memory, memory_type) \
    memory = MakeSharedNoThrow<memory_type>()


///////////////////////////////////////////////////////////////////
//SampleProcess

/**
* SampleProcess
*/
class SampleProcess {
public:
    /**
    * @brief Constructor
    */
    SampleProcess(const char *name):deviceId_(0), context_(nullptr), stream_(nullptr),model_name(name){};

    /**
    * @brief Destructor
    */
    ~SampleProcess();

    /**
    * @brief init reousce
    * @return result
    */
    Result InitResource();
    
    /**
    * @brief encode sample process
    * @param [in] inputimage: input image
    * @return result
    */
    Result MainProcess(void * inputimage);//,uint32_t imagesize

public:
    aclError ret;

    //model
    size_t outputSize;
    //model data
    aclDataBuffer *inputData;
    void *inputBuff = nullptr; //图片文件存放在内存中的地址
    uint32_t inputBuffSize = 0; //输入图片文件占用的内存大小
    aclDataBuffer* outputData;
    void *outputBuffer = nullptr;
    size_t outputBufferSize = 0;
    // output
    float *outData = NULL;

    //utils
    // void readimages();


private:
    void DestroyResource();

    int32_t deviceId_;
    aclrtContext context_;
    aclrtStream stream_;
    aclrtRunMode runMode;
    bool g_isDevice;

    // model
    const char *model_name;
    size_t modelMemSize_,modelWeightSize_; //权值内存大小、工作内存大小
    void *modelMemPtr_; //申请 Device 上模型执行的工作内存
    void *modelWeightPtr_;//申请 Device 上模型执行的权值内存。
    aclmdlDataset *input_;//输入数据结构
    size_t modelInputSize;//申请输入内存
    void *modelInputBuffer = nullptr;//申请输出内存
    aclmdlDataset *output_;//输出数据结构
    size_t outputsize;
    size_t modelOutputSize;//申请输出内存
    void *modelOutputBuffer = nullptr;//申请输入内存
    uint32_t modelId_;
    aclmdlDesc *modelDesc_; //模型的描述信息
    
    
    
};



#endif //ACL_OPTIONS_H