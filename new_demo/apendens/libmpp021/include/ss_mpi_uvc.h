/*
  Copyright (c), 2001-2022, Shenshu Tech. Co., Ltd.
 */

#ifndef __SS_MPI_UVC_H__
#define __SS_MPI_UVC_H__

#include "ot_common.h"
#include "ot_common_video.h"
#include "ot_common_uvc.h"

#ifdef __cplusplus
extern "C" {
#endif

td_s32 ss_mpi_uvc_create_chn(ot_uvc_chn uvc_chn, const ot_uvc_chn_attr *chn_attr);
td_s32 ss_mpi_uvc_destroy_chn(ot_uvc_chn uvc_chn);

td_s32 ss_mpi_uvc_start_chn(ot_uvc_chn uvc_chn);
td_s32 ss_mpi_uvc_stop_chn(ot_uvc_chn uvc_chn);

td_s32 ss_mpi_uvc_get_chn_attr(ot_uvc_chn uvc_chn, ot_uvc_chn_attr *chn_attr);

#ifdef __cplusplus
}
#endif

#endif /* __SS_MPI_UVC_H__ */
