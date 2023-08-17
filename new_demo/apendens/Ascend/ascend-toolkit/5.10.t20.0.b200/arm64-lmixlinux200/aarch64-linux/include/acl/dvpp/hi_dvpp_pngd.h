 /*
 * Copyright (c) Shenshu Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Dvpp Pngd Head File.
 */

#ifndef HI_PNGD_H_
#define HI_PNGD_H_

#include "hi_dvpp_common.h"
#include "hi_dvpp_vb.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif // #ifdef __cplusplus

#define PNGD_MAX_CHN_NUM 128

/*********************************************************************************************/
/* Invalid channel ID. */
#define HI_ERR_PNGD_INVALID_CHN_ID HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_INVALID_CHN_ID)
/* At least one parameter is illegal ,eg, an illegal enumeration value. */
#define HI_ERR_PNGD_ILLEGAL_PARAM  HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_ILLEGAL_PARAM)
/* Channel exists. */
#define HI_ERR_PNGD_EXIST          HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_EXIST)
/* Using a NULL pointer. */
#define HI_ERR_PNGD_NULL_PTR       HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NULL_PTR)
/* Try to enable or initialize system, device or channel, before configure attribute. */
#define HI_ERR_PNGD_NOT_CFG        HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_CFG)
/* Operation is not supported by NOW. */
#define HI_ERR_PNGD_NOT_SUPPORT    HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_SUPPORT)
/* Operation is not permitted, eg, try to change static attribute. */
#define HI_ERR_PNGD_NOT_PERM       HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_PERM)
/* The channel does not exist. */
#define HI_ERR_PNGD_UNEXIST        HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_UNEXIST)
/* Failure caused by malloc memory. */
#define HI_ERR_PNGD_NO_MEM         HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NO_MEM)
/* Failure caused by malloc buffer. */
#define HI_ERR_PNGD_NO_BUF         HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NO_BUF)
/* No data in buffer. */
#define HI_ERR_PNGD_BUF_EMPTY      HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_BUF_EMPTY)
/* No buffer for new data. */
#define HI_ERR_PNGD_BUF_FULL       HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_BUF_FULL)
/* System is not ready, had not initialized or loaded. */
#define HI_ERR_PNGD_SYS_NOT_READY  HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_READY)
/* hardware or software timeout */
#define HI_ERR_PNGD_TIMEOUT        HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_TIMEOUT)
/* System busy */
#define HI_ERR_PNGD_BUSY           HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_BUSY)
/* Bad address, eg. used for copy_from_user & copy_to_user. */
#define HI_ERR_PNGD_BAD_ADDR       HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_BAD_ADDR)
/* Internal system error. */
#define HI_ERR_PNGD_SYS_ERROR      HI_DEFINE_ERR(HI_ID_PNGD, HI_ERR_LEVEL_ERROR, HI_ERR_SYS_ERROR)

typedef hi_s32 hi_pngd_chn;

typedef struct {
    hi_u32 stream_que_cnt;  // reserved
    hi_u64 reserved[4];
} hi_pngd_chn_attr;

/*
 * @brief create png decoder channel
 * @param [in] chn: png decoder channel id [0, PNGD_MAX_CHN_NUM)
 * @param [in] attr: pointer of png decoder channel attribute
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_pngd_create_chn(hi_pngd_chn chn, const hi_pngd_chn_attr *attr);

/*
 * @brief destroy png decoder channel
 * @param [in] chn: png decoder channel id [0, PNGD_MAX_CHN_NUM)
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_pngd_destroy_chn(hi_pngd_chn chn);

/*
 * @brief send stream and outbuffer to png decoder channel
 * @param [in] chn: png decoder channel id [0, PNGD_MAX_CHN_NUM)
 * @param [in] stream: pointer of stream struct
 * @param [in] png_pic_info: pointer of hi_pic_info struct
 * @param [in] milli_sec: -1 is block,0 is no block,other positive number is timeout
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_pngd_send_stream(hi_pngd_chn chn, const hi_img_stream *stream, hi_pic_info *png_pic_info,
    hi_s32 milli_sec);

/*
 * @brief get frame from png decoder channel
 * @param [in] chn: png decoder channel id [0, PNGD_MAX_CHN_NUM)
 * @param [in] milli_sec: -1 is block,0 is no block,other positive number is timeout
 * @param [out] png_pic_info: pointer of pic info struct
 * @param [out] stream: pointer of stream struct
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_pngd_get_image_data(hi_pngd_chn chn, hi_pic_info *png_pic_info, hi_img_stream *stream, hi_s32 milli_sec);

/*
 * @brief get input image's information parsed by dvpp
 * @param [in] stream: stream info pointer
 * @param [out] img_info: parsed image info pointer
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_png_get_image_info(const hi_img_stream *png_stream, hi_img_info *img_info);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif // #ifdef __cplusplus

#endif // #ifndef HI_PNGD_H_
