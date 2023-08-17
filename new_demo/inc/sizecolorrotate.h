#ifndef _SIZE_COLOR_ROTATE_HEADER__
#define _SIZE_COLOR_ROTATE_HEADER__

#include "comdef.h"
#include "offscreen.h"


#ifndef QAF_PI
#define QAF_PI 3.1415926535897932384626433832795
#endif

#ifdef __cplusplus
extern "C" {
#endif
MInt32 qafFlipImageVertical(const MByte * src_data, MByte * dst_data, MInt32 width, MInt32 height, MInt32 linebytes);
MInt32 qafFlipImageHorizon(const MByte * src_data, MByte * dst_data, MInt32 width, MInt32 height, MInt32 linebytes);
MInt32 qafFlipImageVerticalAndHorizon(const MByte * src_data, MByte * dst_data, MInt32 width, MInt32 height, MInt32 linebytes);
MInt32	qafResize_Gray8(MByte *src_data, MInt32 src_width, MInt32 src_height, MInt32 src_linebytes, MByte *dst_data, MInt32 dst_width, MInt32 dst_height, MInt32 dst_linebytes);

MInt32 qaf_normalizeface_fromface(LPOFFSCREEN pImgInfo, MByte * dstimg_data, MInt32 dstimg_width, MInt32 dstimg_height, MInt32 dstimg_linebytes, MInt32 dstimg_channel);

MInt32	qafSizeColorRotate_ToGray8(
									MByte * dst_data,				// [out]
									MInt32 dst_width,				// [out]
									MInt32 dst_height,				// [out]
									MInt32 dst_linebytes,			// [out]
									LPOFFSCREEN pSrcImgInfo,	// [in]
									MInt32 tran,					// [in] flip flag: 1 means flip, and 0 not.
									MInt32 ccw_angle,				// [in] counterclockwise rotation angle
									MPOINT ptBeforeRotate,			// [in]
									MPOINT ptAfterRotate,			// [in]
									MFloat scaling_x,				// [in]
									MFloat scaling_y,				// [in]
									MFloat *p_param					// [out]
									);


MInt32	qafSizeColorRotate_ToGray8_Fix(
									MByte * dst_data,				// [out]
									MInt32 dst_width,				// [out]
									MInt32 dst_height,				// [out]
									MInt32 dst_linebytes,			// [out]
									LPOFFSCREEN pSrcImgInfo,	// [in]
									MInt32 tran,					// [in] flip flag: 1 means flip, and 0 not.
									MInt32 ccw_angle,				// [in] counterclockwise rotation angle
									MPOINT ptBeforeRotate,			// [in]
									MPOINT ptAfterRotate,			// [in]
									MFloat scaling_x,				// [in]
									MFloat scaling_y,				// [in]
									MFloat *p_param					// [out]
									);


MInt32 qafSizeColorRotate_ToBGR888(
									MByte * dst_data,				// [out]
									MInt32 dst_width,				// [out]
									MInt32 dst_height,				// [out]
									MInt32 dst_linebytes,			// [out]
									LPOFFSCREEN pSrcImgInfo,	// [in]
									MInt32 tran,					// [in] flip flag: 1 means flip, and 0 not.
									MInt32 ccw_angle,				// [in] counterclockwise rotation angle
									MPOINT ptBeforeRotate,			// [in]
									MPOINT ptAfterRotate,			// [in]
									MFloat scaling_x,				// [in]
									MFloat scaling_y,				// [in]
								    MFloat *p_param					// [out]
									);

MInt32 qafSizeColorRotate_ToBGR888_Fix(MByte * dst_data,
									   MInt32 dst_width,
									   MInt32 dst_height,
									   MInt32 dst_linebytes,
									   LPOFFSCREEN pSrcImgInfo,	// [in]
									   MInt32 tran,					// [in] flip flag
									   MInt32 ccw_angle,				// [in] counterclockwise rotation angle
									   MPOINT ptBeforeRotate,
									   MPOINT ptAfterRotate,
									   MFloat scaling_x,
									   MFloat scaling_y,
									   MFloat *p_param
									);


#ifdef __cplusplus
}
#endif


#endif //_SIZE_COLOR_ROTATE_HEADER__