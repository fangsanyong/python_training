/*
 * Copyright (c) Shenshu Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Dvpp System Head File.
 */

#ifndef HI_DVPP_SYS_H_
#define HI_DVPP_SYS_H_

#include "hi_dvpp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HI_ERR_SYS_ILLEGAL_PARAM HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_ILLEGAL_PARAM)
#define HI_ERR_SYS_EXIST         HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_EXIST)
#define HI_ERR_SYS_UNEXIST       HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_UNEXIST)
#define HI_ERR_SYS_NULL_PTR      HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_NULL_PTR)
#define HI_ERR_SYS_NOT_SUPPORT   HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_SUPPORT)
#define HI_ERR_SYS_NOT_PERM      HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_PERM)
#define HI_ERR_SYS_NO_MEM        HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_NO_MEM)
#define HI_ERR_SYS_NOT_READY     HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_NOT_READY)
#define HI_ERR_SYS_BUSY          HI_DEFINE_ERR(HI_ID_SYS, HI_ERR_LEVEL_ERROR, HI_ERR_BUSY)

typedef enum {
    HI_DVPP_EPOLL_CTL_ADD = 1,
    HI_DVPP_EPOLL_CTL_MOD = 2,
    HI_DVPP_EPOLL_CTL_DEL = 3,
    HI_DVPP_EPOLL_CTL_BUTT
} hi_dvpp_epoll_ctl_op;

typedef enum {
    HI_DVPP_EPOLL_IN = 1u,
    HI_DVPP_EPOLL_OUT = 1u << 1,
    HI_DVPP_EPOLL_ET = 1u << 31
} hi_dvpp_epoll_event_type;

typedef struct {
    hi_u32 events;
    hi_void *data;
} hi_dvpp_epoll_event;

/*
 * @brief init mpp system
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_init(hi_void);

/*
 * @brief exit mpp system
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_exit(hi_void);

/**
 * @brief Dvpp Epoll create interface
 * @param [in] size: Specify the number of dvpp channel descriptors to monitor,
 *                      currently ignored, and must be positive
 * @param [out] epoll_fd: return the descriptor referring to the new dvpp epoll instance
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_create_epoll(hi_s32 size, hi_s32 *epoll_fd);

/**
 * @brief Dvpp Epoll control interface
 * @param [in] epoll_fd: descriptor of dvpp epoll instance
 * @param [in] operation: operation type for the target dvpp channel descriptor referred by @s32Fd
 * @param [in] fd: descriptor of target dvpp channel
 * @param [in] event: describes the event type wants to monitor, and also the data go with the channel descriptor
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_ctl_epoll(hi_s32 epoll_fd, hi_s32 operation, hi_s32 fd, hi_dvpp_epoll_event *event);

/**
 * @brief Dvpp Epoll wait interface
 * @param [in] epoll_fd: descriptor of dvpp epoll instance
 * @param [out] events: return events that happened
 * @param [in] max_events: maxinum numer of events can return
 * @param [in] timeout: milliseconds the caller can block
 * @param [out] event_num: return the numer of events saved in @pEvents
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_wait_epoll(hi_s32 epoll_fd,
    hi_dvpp_epoll_event *events, hi_s32 max_events, hi_s32 timeout, hi_s32 *event_num);

/**
 * @brief Dvpp Epoll close interface
 * @param [in] epoll_fd:  descriptor of dvpp epoll instance
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_close_epoll(hi_s32 epoll_fd);

/*
 * @brief set csc matrix coefficient
 * @param [in] mode: dvpp module
 * @param [in] chn: chn id
 * @param [in] csc_matrix: csc matrix mode
 * @param [in] csc_coefficient: csc matrix coefficient when use user mode
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_set_chn_csc_matrix(hi_mod_id mode,
    hi_s32 chn, hi_csc_matrix csc_matrix, hi_csc_coefficient *csc_coefficient);

/*
 * @brief get csc matrix coefficient
 * @param [in] mode: dvpp module
 * @param [in] chn: chn id
 * @param [out] csc_matrix: csc matrix mode
 * @param [out] csc_coefficient: csc matrix coefficient for all mode
 * @return success: return 0
 *         fail: return error number
 */
hi_s32 hi_mpi_sys_get_chn_csc_matrix(hi_mod_id mode,
    hi_s32 chn, hi_csc_matrix *csc_matrix, hi_csc_coefficient *csc_coefficient);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif // #ifndef HI_DVPP_SYS_H_
