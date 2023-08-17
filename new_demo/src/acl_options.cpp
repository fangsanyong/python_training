/**
* @file model_process.cpp
*
* Copyright (C) 2020. Huawei Technologies Co., Ltd. All rights reserved.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/
#include "acl_options.h"

using namespace std;

///////////////////////////////////////////////////////////////////
//SampleProcess

SampleProcess::~SampleProcess()
{
    DestroyResource();
}


/**
 * @brief acl初始化，详情见《应用开发指南.pdf》
*/
Result SampleProcess::InitResource()
{
    // ACL init
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx1\n");
    const char *aclConfigPath = "acl.json";
    printf(aclConfigPath);
    printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxx2\n");
    ret = aclInit(aclConfigPath);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclinit failed, ret=%d\n", ret);
        return FAILED;
    }
    INFO_LOG("aclinit success");
    ret = aclrtSetDevice(deviceId_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtSetDevice failed");
        return FAILED;
    }
    INFO_LOG("aclrtSetDevice success");
    ret = aclrtCreateContext(&context_, deviceId_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtCreateContext failed");
        return FAILED;
    }
    INFO_LOG("aclrtCreateContext success");
    ret = aclrtCreateStream(&stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtCreateStream failed");
        return FAILED;
    }
    INFO_LOG("aclrtCreateStream success");
    ret = aclrtGetRunMode(&runMode);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtGetRunMode failed");
        return FAILED;
    }
    INFO_LOG("aclrtGetRunMode success");
    g_isDevice = (runMode == ACL_DEVICE);
    INFO_LOG("g_isDevice: %d", g_isDevice);

    // model load init
    ret = aclmdlQuerySize(model_name, &modelMemSize_, &modelWeightSize_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlQuerySize failed");
        return FAILED;
    }
    INFO_LOG("aclmdlQuerySize success");
    ret = aclrtMalloc(&modelMemPtr_, modelMemSize_, ACL_MEM_MALLOC_NORMAL_ONLY);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtMalloc modelMemPtr failed");
        return FAILED;
    }
    INFO_LOG("aclrtMalloc modelMemPtr success");
    ret = aclrtMalloc(&modelWeightPtr_, modelWeightSize_, ACL_MEM_MALLOC_NORMAL_ONLY);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtMalloc modelWeightPtr failed");
        return FAILED;
    }
    INFO_LOG("aclrtMalloc modelWeightPtr success");
    ret = aclmdlLoadFromFileWithMem(model_name, &modelId_, modelMemPtr_, modelMemSize_,modelWeightPtr_, modelWeightSize_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlLoadFromFileWithMem failed");
        return FAILED;
    }
    INFO_LOG("aclmdlLoadFromFileWithMem success");
    modelDesc_ = aclmdlCreateDesc(); 
    ret = aclmdlGetDesc(modelDesc_, modelId_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlGetDesc modelDesc failed");
        return FAILED;
    }
    INFO_LOG("aclmdlGetDesc modelDesc success");

    // model data init
    //input
    modelInputSize = aclmdlGetInputSizeByIndex(modelDesc_, 0);
    ret = aclrtMalloc(&modelInputBuffer, modelInputSize, ACL_MEM_MALLOC_NORMAL_ONLY);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtMalloc modelInputBuffer failed");
        return FAILED;
    }
    INFO_LOG("aclrtMalloc modelInputBuffer success. modelInputSize: %d",int(modelInputSize));
    input_ = aclmdlCreateDataset();
    inputData = aclCreateDataBuffer(modelInputBuffer, modelInputSize);
    ret = aclmdlAddDatasetBuffer(input_, inputData);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlAddDatasetBuffer failed");
        return FAILED;
    }
    INFO_LOG("aclmdlAddDatasetBuffer success");
    //output
    output_ = aclmdlCreateDataset();

    // INFO_LOG("modelOutputSize: %d", int(modelOutputSize));
    modelOutputSize = aclmdlGetOutputSizeByIndex(modelDesc_, 0);
    INFO_LOG("modelOutputBufferSize: %d", int(modelOutputSize));
    ret = aclrtMalloc(&modelOutputBuffer, modelOutputSize, ACL_MEM_MALLOC_NORMAL_ONLY); 
    if (ret != ACL_SUCCESS) {
    ERROR_LOG("aclrtMalloc outputBuffer failed");
        return FAILED;
    }
    INFO_LOG("aclrtMalloc outputBuffer success");
    outputData = aclCreateDataBuffer(modelOutputBuffer, modelOutputSize); 
    ret = aclmdlAddDatasetBuffer(output_, outputData); 
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlAddDatasetBuffer failed");
        return FAILED;
    }
    INFO_LOG("aclmdlAddDatasetBuffer success");

    // for (size_t i = 0; i < outputSize; ++i) { 
    //     size_t buffer_size = aclmdlGetOutputSizeByIndex(modelDesc_, i); 
    //     void *outputBuffer = nullptr; 
    //     ret = aclrtMalloc(&outputBuffer, buffer_size, ACL_MEM_MALLOC_NORMAL_ONLY); 
    //     if (ret != ACL_SUCCESS) {
    //         ERROR_LOG("aclrtMalloc outputBuffer %d failed",int(i));
    //         return FAILED;
    //     }
    //     INFO_LOG("aclrtMalloc outputBuffer %d success",int(i));
    //     aclDataBuffer* outputData = aclCreateDataBuffer(outputBuffer, buffer_size); 
    //     ret = aclmdlAddDatasetBuffer(output_, outputData); 
    //     if (ret != ACL_SUCCESS) {
    //         ERROR_LOG("aclmdlAddDatasetBuffer %d failed",int(i));
    //         return FAILED;
    //     }
    //     INFO_LOG("aclmdlAddDatasetBuffer %d success",int(i));
    // }

    return SUCCESS;
}

Result SampleProcess::MainProcess(void * inputimage)
{
    // // load images
    if(!g_isDevice)
    {
        ret = aclrtMemcpy(modelInputBuffer, modelInputSize, inputimage, modelInputSize, ACL_MEMCPY_HOST_TO_DEVICE);
        if (ret != ACL_SUCCESS) {
            ERROR_LOG("copy input failed");
            return FAILED;
        }
        //(void)aclrtFreeHost(inputBuff);inputBuffSize
    }else
    {
        ret = aclrtMemcpy(modelInputBuffer, modelInputSize, inputimage, modelInputSize, ACL_MEMCPY_DEVICE_TO_DEVICE);
        if (ret != ACL_SUCCESS) {
            ERROR_LOG("copy input failed");
            return FAILED;
        }
    }

    ret = aclmdlExecute(modelId_, input_, output_); // !!!-> run model <-!!! 
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlExecute failed");
        return FAILED;
    }
    INFO_LOG("aclmdlExecute success");
    // ret = aclmdlExecute(modelId_, input_, output_); // !!!-> run model <-!!! 
    // if (ret != ACL_SUCCESS) {
    //     ERROR_LOG("aclmdlExecute failed");
    //     return FAILED;
    // }

    // get output data
    aclDataBuffer* dataBuffer = aclmdlGetDatasetBuffer(output_,0); 
    void* data = aclGetDataBufferAddr(dataBuffer);
    //ize_t len = aclGetDataBufferSizeV2(dataBuffer);
    outData = reinterpret_cast<float*>(data);

    //INFO_LOG("outData: %6f,%6f,%6f,%6f,%6f",outData[0],outData[1],outData[2],outData[3],outData[4],outData[5]);


    return SUCCESS;
}

void SampleProcess::DestroyResource()
{
    INFO_LOG("____________release___________");
    INFO_LOG("  ______                      ");
    INFO_LOG(" /      \\                     ");
    INFO_LOG("|  $$$$$$\\  ______  ________ ");
    INFO_LOG("| $$  | $$ /      \\|        \\");
    INFO_LOG("| $$  | $$|  $$$$$$\\$$$$$$$$");
    INFO_LOG("| $$  | $$| $$   \\$$ /    $$ ");
    INFO_LOG("| $$__/ $$| $$      /  $$$$_ ");
    INFO_LOG(" \\$$    $$| $$     |  $$    \\ ");
    INFO_LOG("  \\$$$$$$  \\$$      \\$$$$$$$$ ");

    //destroy data
 
    aclDataBuffer *dataBuffer = aclmdlGetDatasetBuffer(input_, 0); 
    (void)aclDestroyDataBuffer(dataBuffer); 
    (void)aclmdlDestroyDataset(input_); 
    input_ = nullptr; 
    aclrtFree(modelInputBuffer); 
    dataBuffer = aclmdlGetDatasetBuffer(output_, 0); 
    void* data = aclGetDataBufferAddr(dataBuffer); 
    (void)aclrtFree(data); 
    (void)aclDestroyDataBuffer(dataBuffer); 
    (void)aclmdlDestroyDataset(output_); 
    output_ = nullptr;


    //destroy model
    ret = aclrtDestroyStream(stream_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtDestroyStream failed");
    }
    INFO_LOG("aclrtDestroyStream success");
    ret = aclrtDestroyContext(context_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtDestroyContext failed");
    }
    INFO_LOG("aclrtDestroyContext success");
    ret = aclrtResetDevice(deviceId_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclrtResetDevice failed");
    }
    INFO_LOG("aclrtResetDevice success");

    ret = aclFinalize();
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclFinalize failed");
    }
    INFO_LOG("aclFinalize success");

    // destroy acl
    ret = aclmdlUnload(modelId_);
    if (ret != ACL_SUCCESS) {
        ERROR_LOG("aclmdlUnload failed");
    }
    INFO_LOG("aclmdlUnload success");
    // if (modelDesc_ != nullptr) { 
    //     (void)aclmdlDestroyDesc(modelDesc_); 
    //     modelDesc_ = nullptr; 
    // } 
    // if (modelMemPtr_ != nullptr) { 
    //     (void)aclrtFree(modelMemPtr_); 
    //     modelMemPtr_= nullptr; 
    //     modelMemSize_ = 0; 
    // }
    // if (modelWeightPtr_ != nullptr) { 
    //     (void)aclrtFree(modelWeightPtr_); 
    //     modelWeightPtr_ = nullptr; 
    //     modelWeightSize_ = 0; 
    // }

    INFO_LOG("Destroy finish");
}