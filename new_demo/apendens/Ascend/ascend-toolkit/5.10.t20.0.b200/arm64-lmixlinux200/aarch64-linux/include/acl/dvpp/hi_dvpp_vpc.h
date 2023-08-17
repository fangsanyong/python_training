 /*
 * Copyright (c) Shenshu Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Dvpp Vpc Head File.
 */

#ifndef HI_VPC_H_
#define HI_VPC_H_

#include "hi_dvpp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif // #ifdef __cplusplus

#define VPC_MAX_CHN_NUM 512

/*********************************************************************************************/
/* invlalid channel ID */
#define HI_ERR_VPC_INVALID_CHN_ID HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_INVALID_CHN_ID)
/* at lease one parameter is illagal ,eg, an illegal enumeration value  */
#define HI_ERR_VPC_ILLEGAL_PARAM  HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_ILLEGAL_PARAM)
/* channel exists */
#define HI_ERR_VPC_EXIST          HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_EXIST)
/* using a NULL point */
#define HI_ERR_VPC_NULL_PTR       HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NULL_PTR)
/* try to enable or initialize system,device or channel, before configuring attribute */
#define HI_ERR_VPC_NOT_CFG        HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_CFG)
/* operation is not supported by NOW */
#define HI_ERR_VPC_NOT_SUPPORT    HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_SUPPORT)
/* operation is not permitted ,eg, try to change stati attribute */
#define HI_ERR_VPC_NOT_PERM       HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_PERM)
/* the channel is not existed  */
#define HI_ERR_VPC_UNEXIST        HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_UNEXIST)
/* failure caused by malloc memory */
#define HI_ERR_VPC_NO_MEM         HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NO_MEM)
/* failure caused by malloc buffer */
#define HI_ERR_VPC_NO_BUF         HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NO_BUF)
/* no data in buffer */
#define HI_ERR_VPC_BUF_EMPTY      HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_BUF_EMPTY)
/* no buffer for new data */
#define HI_ERR_VPC_BUF_FULL       HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_BUF_FULL)
/* system is not ready,had not initialized or loaded */
#define HI_ERR_VPC_SYS_NOT_READY  HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_READY)
/* system busy */
#define HI_ERR_VPC_BUSY           HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_BUSY)
/* bad address,  eg. used for copy_from_user & copy_to_user   */
#define HI_ERR_VPC_BAD_ADDR       HI_DEFINE_ERR(HI_ID_VPC, HI_ERR_LEVEL_ERROR, HI_ERR_BAD_ADDR)

typedef hi_s32 hi_vpc_chn;

typedef enum {
    HI_BORDER_CONSTANT = 0,
    HI_BORDER_REPLICATE,
    HI_BORDER_REFLECT,
    HI_BORDER_REFLECT_101
} hi_vpc_bord_type;

typedef struct {
    hi_double val[4];
} hi_vpc_scalar;

typedef struct {
    hi_u32 top;
    hi_u32 bottom;
    hi_u32 left;
    hi_u32 right;
    hi_vpc_bord_type border_type;
    hi_vpc_scalar scalar_value;
} hi_vpc_make_border_info;

typedef struct {
    hi_void* picture_address;
    hi_u32 picture_buffer_size;
    hi_u32 picture_width;
    hi_u32 picture_height;
    hi_u32 picture_width_stride;
    hi_u32 picture_height_stride;
    hi_pixel_format picture_format;
} hi_vpc_pic_info;

typedef struct {
    hi_u32 top_offset;
    hi_u32 left_offset;
    hi_u32 crop_width;
    hi_u32 crop_height;
} hi_vpc_crop_region;

typedef struct {
    hi_vpc_pic_info dest_pic_info;
    hi_vpc_crop_region crop_region;
} hi_vpc_crop_region_info;

typedef struct {
    hi_u32 resize_width;
    hi_u32 resize_height;
    hi_u32 interpolation;
} hi_vpc_resize_info;

typedef struct {
    hi_vpc_pic_info dest_pic_info;
    hi_vpc_crop_region crop_region;
    hi_vpc_resize_info resize_info;
} hi_vpc_crop_resize_region;

typedef struct {
    hi_vpc_pic_info dest_pic_info;
    hi_vpc_crop_region crop_region;
    hi_vpc_resize_info resize_info;
    hi_u32 dest_top_offset;
    hi_u32 dest_left_offset;
} hi_vpc_crop_resize_paste_region;

typedef struct {
    hi_vpc_pic_info dest_pic_info;
    hi_vpc_crop_region crop_region;
    hi_vpc_resize_info resize_info;
    hi_u32 dest_top_offset;
    hi_u32 dest_left_offset;
    hi_vpc_bord_type border_type;
    hi_vpc_scalar scalar_value;
} hi_vpc_crop_resize_border_region;

typedef struct {
    hi_s32 attr; // RW; reserved
    hi_u32 pic_width; // RW; max pic width
    hi_u32 pic_height; // RW; max pic height
} hi_vpc_chn_attr;

typedef struct {
    hi_u32 histogram_y_or_r[256]; // 256 level statistics of Y or R component
    hi_u32 histogram_u_or_g[256]; // 256 level statistics of U or G component
    hi_u32 histogram_v_or_b[256]; // 256 level statistics of V or B component
} hi_vpc_histogram_config;

typedef struct {
    hi_u8 map_value_y_or_r[256]; // remap value of Y or R component
    hi_u8 map_value_u_or_g[256]; // remap value of U or G component
    hi_u8 map_value_v_or_b[256]; // remap value of V or B component
} hi_vpc_lut_remap;

/*
 * @brief create vpc channel
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] attr: pointer of vpc channel attribute
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_create_chn(hi_vpc_chn chn, const hi_vpc_chn_attr *attr);

/*
 * @brief create system vpc channel for single channel multi-core acceleration
 * @param [out] chnl: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] attr: pointer of vpc channel attribute
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_sys_create_chn(hi_vpc_chn *chnl, const hi_vpc_chn_attr *attr);

/*
 * @brief destroy vpc channel
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_destroy_chn(hi_vpc_chn chn);

/*
 * @brief crop
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] crop_info: array of vpc crop region info
 * @param [in] count: array length of crop_info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of crop interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_crop(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_crop_region_info crop_info[],
    hi_u32 count, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief resize
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic: pointer of vpc dest picture info
 * @param [in] fx: width resize info
 * @param [in] fy: height resize info
 * @param [in] interpolation: resize mode, support bilinear/nearest neighbor
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of resize interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_resize(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_pic_info *dest_pic,
    hi_double fx, hi_double fy, hi_u32 interpolation, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief crop and resize
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] crop_resize_info: array of vpc crop_resize region info
 * @param [in] count: array length of crop_resize_info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of crop_resize interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_crop_resize(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic,
    hi_vpc_crop_resize_region crop_resize_info[], hi_u32 count, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief crop and resize and paste
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] crop_resize_paste_info: array of vpc crop_resize_paste region info
 * @param [in] count: array length of crop_resize_paste_info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of crop_resize_paste interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_crop_resize_paste(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic,
    hi_vpc_crop_resize_paste_region crop_resize_paste_info[], hi_u32 count, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief crop and resize and paste interface for batch pictures batch regions
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: array of pointer of vpc source picture info
 * @param [in] crop_resize_paste_info: array of vpc crop_resize_paste region info
 * @param [in] count: the num of regions correspond to source_pic
 * @param [in] pic_num: number os pictures
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of batch_crop_resize_paste interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_batch_crop_resize_paste(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic[], hi_u32 pic_num,
    hi_vpc_crop_resize_paste_region crop_resize_paste_info[], hi_u32 count[], hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief crop and resize and make border
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] crop_resize_make_border_info: array of vpc crop_resize_make_border region info
 * @param [in] count: array length of crop_resize_make_border
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of crop_resize_make_border interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_crop_resize_make_border(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic,
    hi_vpc_crop_resize_border_region crop_resize_make_border_info[], hi_u32 count, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief crop and resize and make border for batch pictures and batch regions
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: arrary of pointer of vpc source picture info
 * @param [in] crop_resize_make_border_info: array of vpc crop_resize_make_border region info
 * @param [in] count: the num of regions correspond to source_pic
 * @param [in] pic_num: number os pictures
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of batch_crop_resize_make_border interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_batch_crop_resize_make_border(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic[], hi_u32 pic_num,
    hi_vpc_crop_resize_border_region crop_resize_make_border_info[], hi_u32 count[], hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief convert color
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic: pointer of vpc dest picture info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of convert_color interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_convert_color(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_pic_info *dest_pic,
    hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief convert color to yuv420sp
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic: pointer of vpc dest picture info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of convert_color_to_yuv420 interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_convert_color_to_yuv420(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_pic_info *dest_pic,
    hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief border padding
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic: pointer of vpc dest picture info
 * @param [in] make_border_info: boundary fill information
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of copy_make_border interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_copy_make_border(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_pic_info *dest_pic,
    hi_vpc_make_border_info make_border_info, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief multi-level pyramid down
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic[]: pointer array of vpc dest picture info
 * @param [in] filterLevel: pyramid level [1, 4]
 * @param [in] gaussian_filter: convolution kernel of 5x5
 * @param [in] make_border_info: boundary padding algorithm and boundary pixel value info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of pyrdown interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_pyrdown(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic,
    hi_vpc_pic_info dest_pic[], hi_u32 filter_level, hi_s8 gaussian_filter[][5],
    hi_u16 divisor, hi_vpc_make_border_info make_border_info, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief calculate histogram
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] hist_config: histogram result
 * @param [out] task_id: task id of calc_hist interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_calc_hist(hi_vpc_chn chn, const hi_vpc_pic_info *source_pic, hi_vpc_histogram_config *hist_config,
    hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief equalization image
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] source_pic: pointer of vpc source picture info
 * @param [in] dest_pic: pointer of vpc dest picture info
 * @param [in] lut_remap: remap value of YUV or RGB component
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @param [out] task_id: task id of equalize_hist interface
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_equalize_hist(hi_vpc_chn chn, const hi_vpc_pic_info* source_pic, hi_vpc_pic_info *dest_pic,
    const hi_vpc_lut_remap *lut_remap, hi_u32 *task_id, hi_s32 milli_sec);

/*
 * @brief query whether the task has been completed, base on task_id
 * @param [in] chn: vpc channel id [0, VPC_MAX_CHN_NUM)
 * @param [in] task_id: task id generated by calling image processing interface
 * @param [in] milli_sec: -1 is block, 0 is no block, other positive number is timeout
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_vpc_get_process_result(hi_vpc_chn chn, hi_u32 task_id, hi_s32 milli_sec);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // #ifdef __cplusplus

#endif // #ifndef HI_VPC_H_
