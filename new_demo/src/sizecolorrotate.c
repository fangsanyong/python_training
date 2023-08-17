#include "math.h"
#include "sizecolorrotate.h"	
#include<string.h>

#undef CAST_8U
#define CAST_8U(t)  (MByte)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)

#undef PRECISION
#define PRECISION 16

#undef BGR2Y_COEF_B
#undef BGR2Y_COEF_G
#undef BGR2Y_COEF_R
#define BGR2Y_COEF_B 0.114
#define BGR2Y_COEF_G 0.587
#define BGR2Y_COEF_R 0.299

#undef BGR2Y_COEF_B_FIX
#undef BGR2Y_COEF_G_FIX
#undef BGR2Y_COEF_R_FIX
#define BGR2Y_COEF_B_FIX ((MInt32)(BGR2Y_COEF_B*(1<<PRECISION)))
#define BGR2Y_COEF_G_FIX ((MInt32)(BGR2Y_COEF_G*(1<<PRECISION)))
#define BGR2Y_COEF_R_FIX ((MInt32)(BGR2Y_COEF_R*(1<<PRECISION)))

#undef table_u_b
#undef table_v_r
#undef table_u_g
#undef table_v_g
#define table_u_b table_u_b_i32
#define table_v_r table_v_r_i32
#define table_u_g table_u_g_i32
#define table_v_g table_v_g_i32	

static const int table_u_b_i32[] = 
{
-226,-225,-223,-221,-219,-218,-216,-214,-212,-210,-209,-207,-205,-203,-202,-200,-198,-196,-195,-193,
-191,-189,-187,-186,-184,-182,-180,-179,-177,-175,-173,-171,-170,-168,-166,-164,-163,-161,-159,-157,
-156,-154,-152,-150,-148,-147,-145,-143,-141,-140,-138,-136,-134,-132,-131,-129,-127,-125,-124,-122,
-120,-118,-117,-115,-113,-111,-109,-108,-106,-104,-102,-101,-99,-97,-95,-93,-92,-90,-88,-86,
-85,-83,-81,-79,-78,-76,-74,-72,-70,-69,-67,-65,-63,-62,-60,-58,-56,-54,-53,-51,
-49,-47,-46,-44,-42,-40,-39,-37,-35,-33,-31,-30,-28,-26,-24,-23,-21,-19,-17,-15,
-14,-12,-10,-8,-7,-5,-3,-1,0,1,3,5,7,8,10,12,14,15,17,19,
21,23,24,26,28,30,31,33,35,37,39,40,42,44,46,47,49,51,53,54,
56,58,60,62,63,65,67,69,70,72,74,76,78,79,81,83,85,86,88,90,
92,93,95,97,99,101,102,104,106,108,109,111,113,115,117,118,120,122,124,125,
127,129,131,132,134,136,138,140,141,143,145,147,148,150,152,154,156,157,159,161,
163,164,166,168,170,171,173,175,177,179,180,182,184,186,187,189,191,193,195,196,
198,200,202,203,205,207,209,210,212,214,216,218,219,221,223,225
};

static const int table_v_r_i32[] = 
{
-179,-178,-176,-175,-173,-172,-171,-169,-168,-166,-165,-164,-162,-161,-159,-158,-157,-155,-154,-152,
-151,-150,-148,-147,-145,-144,-143,-141,-140,-138,-137,-136,-134,-133,-131,-130,-129,-127,-126,-124,
-123,-122,-120,-119,-117,-116,-115,-113,-112,-110,-109,-107,-106,-105,-103,-102,-100,-99,-98,-96,
-95,-93,-92,-91,-89,-88,-86,-85,-84,-82,-81,-79,-78,-77,-75,-74,-72,-71,-70,-68,
-67,-65,-64,-63,-61,-60,-58,-57,-56,-54,-53,-51,-50,-49,-47,-46,-44,-43,-42,-40,
-39,-37,-36,-35,-33,-32,-30,-29,-28,-26,-25,-23,-22,-21,-19,-18,-16,-15,-14,-12,
-11,-9,-8,-7,-5,-4,-2,-1,0,1,2,4,5,7,8,9,11,12,14,15,
16,18,19,21,22,23,25,26,28,29,30,32,33,35,36,37,39,40,42,43,
44,46,47,49,50,51,53,54,56,57,58,60,61,63,64,65,67,68,70,71,
72,74,75,77,78,79,81,82,84,85,86,88,89,91,92,93,95,96,98,99,
100,102,103,105,106,107,109,110,112,113,115,116,117,119,120,122,123,124,126,127,
129,130,131,133,134,136,137,138,140,141,143,144,145,147,148,150,151,152,154,155,
157,158,159,161,162,164,165,166,168,169,171,172,173,175,176,178
};

static const int table_u_g_i32[] = 
{
44,43,43,43,42,42,41,41,41,40,40,40,39,39,39,38,38,38,37,37,
37,36,36,36,35,35,35,34,34,34,33,33,33,32,32,31,31,31,30,30,
30,29,29,29,28,28,28,27,27,27,26,26,26,25,25,25,24,24,24,23,
23,23,22,22,22,21,21,20,20,20,19,19,19,18,18,18,17,17,17,16,
16,16,15,15,15,14,14,14,13,13,13,12,12,12,11,11,11,10,10,9,
9,9,8,8,8,7,7,7,6,6,6,5,5,5,4,4,4,3,3,3,
2,2,2,1,1,1,0,0,0,0,0,-1,-1,-1,-2,-2,-2,-3,-3,-3,
-4,-4,-4,-5,-5,-5,-6,-6,-6,-7,-7,-7,-8,-8,-8,-9,-9,-9,-10,-10,
-11,-11,-11,-12,-12,-12,-13,-13,-13,-14,-14,-14,-15,-15,-15,-16,-16,-16,-17,-17,
-17,-18,-18,-18,-19,-19,-19,-20,-20,-20,-21,-21,-22,-22,-22,-23,-23,-23,-24,-24,
-24,-25,-25,-25,-26,-26,-26,-27,-27,-27,-28,-28,-28,-29,-29,-29,-30,-30,-30,-31,
-31,-31,-32,-32,-33,-33,-33,-34,-34,-34,-35,-35,-35,-36,-36,-36,-37,-37,-37,-38,
-38,-38,-39,-39,-39,-40,-40,-40,-41,-41,-41,-42,-42,-43,-43,-43
};

static const int table_v_g_i32[] = 
{
91,90,89,89,88,87,87,86,85,84,84,83,82,82,81,80,79,79,78,77,
77,76,75,74,74,73,72,72,71,70,69,69,68,67,67,66,65,64,64,63,
62,62,61,60,59,59,58,57,57,56,55,54,54,53,52,52,51,50,49,49,
48,47,47,46,45,44,44,43,42,42,41,40,39,39,38,37,37,36,35,34,
34,33,32,32,31,30,29,29,28,27,27,26,25,24,24,23,22,22,21,20,
19,19,18,17,17,16,15,14,14,13,12,12,11,10,9,9,8,7,7,6,
5,4,4,3,2,2,1,0,0,0,-1,-2,-2,-3,-4,-4,-5,-6,-7,-7,
-8,-9,-9,-10,-11,-12,-12,-13,-14,-14,-15,-16,-17,-17,-18,-19,-19,-20,-21,-22,
-22,-23,-24,-24,-25,-26,-27,-27,-28,-29,-29,-30,-31,-32,-32,-33,-34,-34,-35,-36,
-37,-37,-38,-39,-39,-40,-41,-42,-42,-43,-44,-44,-45,-46,-47,-47,-48,-49,-49,-50,
-51,-52,-52,-53,-54,-54,-55,-56,-57,-57,-58,-59,-59,-60,-61,-62,-62,-63,-64,-64,
-65,-66,-67,-67,-68,-69,-69,-70,-71,-72,-72,-73,-74,-74,-75,-76,-77,-77,-78,-79,
-79,-80,-81,-82,-82,-83,-84,-84,-85,-86,-87,-87,-88,-89,-89,-90
};

MInt32 qaf_normalizeface_fromface(LPOFFSCREEN pImgInfo, MByte * dstimg_data, MInt32 dstimg_width, MInt32 dstimg_height, MInt32 dstimg_linebytes, MInt32 dstimg_channel)
{
	MPOINT FaceCenter = {0}, FaceCenterInSmall = {0};
	MInt32 lCCWAngle = 0;
	MFloat scaling_x = 1.0, scaling_y = 1.0;
	
	memset(dstimg_data, 0, dstimg_height*dstimg_linebytes);

	// FaceSize_Width = FaceRect.right - FaceRect.left + 1;
	// FaceSize_Height = FaceRect.bottom - FaceRect.top + 1;

	scaling_x = ((MFloat)dstimg_width)/pImgInfo->i32Width;
	scaling_y = ((MFloat)dstimg_height)/pImgInfo->i32Height;
	
	FaceCenter.x = (pImgInfo->i32Width);///2;
	FaceCenter.y = (pImgInfo->i32Height);///2;
	
	FaceCenterInSmall.x = dstimg_width;/// 2;
	FaceCenterInSmall.y = dstimg_height;// / 2;

	qafSizeColorRotate_ToBGR888(dstimg_data, dstimg_width, dstimg_height, dstimg_linebytes, pImgInfo, 0, lCCWAngle, FaceCenter, FaceCenterInSmall, scaling_x, scaling_y, 0);

	return 0;	
}

MInt32 qafSizeColorRotate_ToBGR888(MByte * dst_data,
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
									MFloat *p_param					// [out]
									)
{
	MInt32 x, y;
	MDouble val_cos, val_sin;
	MDouble param1, param2, param3, param4;
	MDouble f1, f2;
	MDouble rad;
	MInt32 src_width = pSrcImgInfo->i32Width;
	MInt32 src_height = pSrcImgInfo->i32Height;

	MInt32 valB, valG, valR;
	
	rad = QAF_PI*ccw_angle/180;
	
	val_cos = cos(rad);
	val_sin = sin(rad);
	
	param1 = val_cos / scaling_x;
	param2 = -val_sin / scaling_x;
	param3 = val_sin / scaling_y;
	param4 = val_cos / scaling_y;
	
	f1 = ptBeforeRotate.x - (ptAfterRotate.x*param1 + ptAfterRotate.y*param3);
	f2 = ptBeforeRotate.y - (ptAfterRotate.x*param2 + ptAfterRotate.y*param4);

	switch (pSrcImgInfo->u32PixelArrayFormat)
	{
	case PAF_NV21:
		{
			MByte * src_data_Y = pSrcImgInfo->ppu8Plane[0];
			MByte * src_data_UV = pSrcImgInfo->ppu8Plane[1];
			MInt32 src_linebytes_Y = pSrcImgInfo->pi32Pitch[0];
			MInt32 src_linebytes_UV = pSrcImgInfo->pi32Pitch[1];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					MInt32 nv21_y0, nv21_y1;
					MInt32 srcB0, srcG0, srcR0;
					MInt32 srcB1, srcG1, srcR1;
					MInt32 srcB2, srcG2, srcR2;
					MInt32 srcB3, srcG3, srcR3;
					MInt32 srcY0, srcU0, srcV0;
					MInt32 srcY1, srcU1, srcV1;
					MInt32 srcY2, srcU2, srcV2;
					MInt32 srcY3, srcU3, srcV3;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						nv21_y0 = iys;
						nv21_y1 = nv21_y0 + 1;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						nv21_y0 = nv21_y1 = iys;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;

						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						srcV0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						
						srcY1 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs+1];
						srcU1 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)+1];
						srcV1 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)];

						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						srcV2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)];

						srcY3 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs+1];
						srcU3 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)+1];
						srcV3 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)];

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];

						srcB1 = srcY1 + table_u_b[srcU1];
						srcG1 = srcY1 + table_u_g[srcU1] + table_v_g[srcV1];
						srcR1 = srcY1 + table_v_r[srcV1];

						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						srcB3 = srcY3 + table_u_b[srcU3];
						srcG3 = srcY3 + table_u_g[srcU3] + table_v_g[srcV3];
						srcR3 = srcY3 + table_v_r[srcV3];
						
						p0 = srcB0*a1 + srcB1*a;
						p1 = srcB2*a1 + srcB3*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = srcG0*a1 + srcG1*a;
						p1 = srcG2*a1 + srcG3*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = srcR0*a1 + srcR1*a;
						p1 = srcR2*a1 + srcR3*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						srcV0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						
						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						srcV2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						
						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];
						
						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						valB = (MInt32)(srcB0*b1 + srcB2*b);
						valG = (MInt32)(srcG0*b1 + srcG2*b);
						valR = (MInt32)(srcR0*b1 + srcR2*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;

	case PAF_NV12:
		{
			MByte * src_data_Y = pSrcImgInfo->ppu8Plane[0];
			MByte * src_data_UV = pSrcImgInfo->ppu8Plane[1];
			MInt32 src_linebytes_Y = pSrcImgInfo->pi32Pitch[0];
			MInt32 src_linebytes_UV = pSrcImgInfo->pi32Pitch[1];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					MInt32 nv21_y0, nv21_y1;
					MInt32 srcB0, srcG0, srcR0;
					MInt32 srcB1, srcG1, srcR1;
					MInt32 srcB2, srcG2, srcR2;
					MInt32 srcB3, srcG3, srcR3;
					MInt32 srcY0, srcU0, srcV0;
					MInt32 srcY1, srcU1, srcV1;
					MInt32 srcY2, srcU2, srcV2;
					MInt32 srcY3, srcU3, srcV3;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						nv21_y0 = iys;
						nv21_y1 = nv21_y0 + 1;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						nv21_y0 = nv21_y1 = iys;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;

						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						srcV0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						
						srcY1 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs+1];
						srcU1 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)];
						srcV1 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)+1];

						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						srcV2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];

						srcY3 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs+1];
						srcU3 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)];
						srcV3 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[(((ixs+1)>>1)<<1)+1];

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];

						srcB1 = srcY1 + table_u_b[srcU1];
						srcG1 = srcY1 + table_u_g[srcU1] + table_v_g[srcV1];
						srcR1 = srcY1 + table_v_r[srcV1];

						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						srcB3 = srcY3 + table_u_b[srcU3];
						srcG3 = srcY3 + table_u_g[srcU3] + table_v_g[srcV3];
						srcR3 = srcY3 + table_v_r[srcV3];
						
						p0 = srcB0*a1 + srcB1*a;
						p1 = srcB2*a1 + srcB3*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = srcG0*a1 + srcG1*a;
						p1 = srcG2*a1 + srcG3*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = srcR0*a1 + srcR1*a;
						p1 = srcR2*a1 + srcR3*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						srcV0 = (src_data_UV + nv21_y0/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						
						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)];
						srcV2 = (src_data_UV + nv21_y1/2 * src_linebytes_UV)[((ixs>>1)<<1)+1];
						
						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];
						
						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						valB = (MInt32)(srcB0*b1 + srcB2*b);
						valG = (MInt32)(srcG0*b1 + srcG2*b);
						valR = (MInt32)(srcR0*b1 + srcR2*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;

	case PAF_YV12:
		{
			MByte * src_data_Y = pSrcImgInfo->ppu8Plane[0];
			MByte * src_data_U = pSrcImgInfo->ppu8Plane[2];
			MByte * src_data_V = pSrcImgInfo->ppu8Plane[1];
			MInt32 src_linebytes_Y = pSrcImgInfo->pi32Pitch[0];
			MInt32 src_linebytes_U = pSrcImgInfo->pi32Pitch[2];
			MInt32 src_linebytes_V = pSrcImgInfo->pi32Pitch[1];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					MInt32 nv21_y0, nv21_y1;
					MInt32 srcB0, srcG0, srcR0;
					MInt32 srcB1, srcG1, srcR1;
					MInt32 srcB2, srcG2, srcR2;
					MInt32 srcB3, srcG3, srcR3;
					MInt32 srcY0, srcU0, srcV0;
					MInt32 srcY1, srcU1, srcV1;
					MInt32 srcY2, srcU2, srcV2;
					MInt32 srcY3, srcU3, srcV3;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						nv21_y0 = iys;
						nv21_y1 = nv21_y0 + 1;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						nv21_y0 = nv21_y1 = iys;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;

						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_U + nv21_y0/2 * src_linebytes_U)[(ixs>>1)];
						srcV0 = (src_data_V + nv21_y0/2 * src_linebytes_V)[(ixs>>1)];
						
						srcY1 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs+1];
						srcU1 = (src_data_U + nv21_y0/2 * src_linebytes_U)[((ixs+1)>>1)];
						srcV1 = (src_data_V + nv21_y0/2 * src_linebytes_V)[((ixs+1)>>1)];

						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_U + nv21_y1/2 * src_linebytes_U)[(ixs>>1)];
						srcV2 = (src_data_V + nv21_y1/2 * src_linebytes_V)[(ixs>>1)];

						srcY3 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs+1];
						srcU3 = (src_data_U + nv21_y1/2 * src_linebytes_U)[((ixs+1)>>1)];
						srcV3 = (src_data_V + nv21_y1/2 * src_linebytes_V)[((ixs+1)>>1)];

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];

						srcB1 = srcY1 + table_u_b[srcU1];
						srcG1 = srcY1 + table_u_g[srcU1] + table_v_g[srcV1];
						srcR1 = srcY1 + table_v_r[srcV1];

						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						srcB3 = srcY3 + table_u_b[srcU3];
						srcG3 = srcY3 + table_u_g[srcU3] + table_v_g[srcV3];
						srcR3 = srcY3 + table_v_r[srcV3];

						p0 = srcB0*a1 + srcB1*a;
						p1 = srcB2*a1 + srcB3*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = srcG0*a1 + srcG1*a;
						p1 = srcG2*a1 + srcG3*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = srcR0*a1 + srcR1*a;
						p1 = srcR2*a1 + srcR3*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_U + nv21_y0/2 * src_linebytes_U)[(ixs>>1)];
						srcV0 = (src_data_V + nv21_y0/2 * src_linebytes_V)[(ixs>>1)];
						
						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_U + nv21_y1/2 * src_linebytes_U)[(ixs>>1)];
						srcV2 = (src_data_V + nv21_y1/2 * src_linebytes_V)[(ixs>>1)];
						
						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];
						
						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						valB = (MInt32)(srcB0*b1 + srcB2*b);
						valG = (MInt32)(srcG0*b1 + srcG2*b);
						valR = (MInt32)(srcR0*b1 + srcR2*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;
		
	case  PAF_I420:
		{
			MByte * src_data_Y = pSrcImgInfo->ppu8Plane[0];
			MByte * src_data_U = pSrcImgInfo->ppu8Plane[1];
			MByte * src_data_V = pSrcImgInfo->ppu8Plane[2];
			MInt32 src_linebytes_Y = pSrcImgInfo->pi32Pitch[0];
			MInt32 src_linebytes_U = pSrcImgInfo->pi32Pitch[1];
			MInt32 src_linebytes_V = pSrcImgInfo->pi32Pitch[2];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					MInt32 nv21_y0, nv21_y1;
					MInt32 srcB0, srcG0, srcR0;
					MInt32 srcB1, srcG1, srcR1;
					MInt32 srcB2, srcG2, srcR2;
					MInt32 srcB3, srcG3, srcR3;
					MInt32 srcY0, srcU0, srcV0;
					MInt32 srcY1, srcU1, srcV1;
					MInt32 srcY2, srcU2, srcV2;
					MInt32 srcY3, srcU3, srcV3;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						nv21_y0 = iys;
						nv21_y1 = nv21_y0 + 1;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						nv21_y0 = nv21_y1 = iys;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;

						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_U + nv21_y0/2 * src_linebytes_U)[(ixs>>1)];
						srcV0 = (src_data_V + nv21_y0/2 * src_linebytes_V)[(ixs>>1)];
						
						srcY1 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs+1];
						srcU1 = (src_data_U + nv21_y0/2 * src_linebytes_U)[((ixs+1)>>1)];
						srcV1 = (src_data_V + nv21_y0/2 * src_linebytes_V)[((ixs+1)>>1)];

						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_U + nv21_y1/2 * src_linebytes_U)[(ixs>>1)];
						srcV2 = (src_data_V + nv21_y1/2 * src_linebytes_V)[(ixs>>1)];

						srcY3 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs+1];
						srcU3 = (src_data_U + nv21_y1/2 * src_linebytes_U)[((ixs+1)>>1)];
						srcV3 = (src_data_V + nv21_y1/2 * src_linebytes_V)[((ixs+1)>>1)];

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];

						srcB1 = srcY1 + table_u_b[srcU1];
						srcG1 = srcY1 + table_u_g[srcU1] + table_v_g[srcV1];
						srcR1 = srcY1 + table_v_r[srcV1];

						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						srcB3 = srcY3 + table_u_b[srcU3];
						srcG3 = srcY3 + table_u_g[srcU3] + table_v_g[srcV3];
						srcR3 = srcY3 + table_v_r[srcV3];
						
						p0 = srcB0*a1 + srcB1*a;
						p1 = srcB2*a1 + srcB3*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = srcG0*a1 + srcG1*a;
						p1 = srcG2*a1 + srcG3*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = srcR0*a1 + srcR1*a;
						p1 = srcR2*a1 + srcR3*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						srcY0 = (src_data_Y + nv21_y0 * src_linebytes_Y)[ixs];
						srcU0 = (src_data_U + nv21_y0/2 * src_linebytes_U)[(ixs>>1)];
						srcV0 = (src_data_V + nv21_y0/2 * src_linebytes_V)[(ixs>>1)];
						
						srcY2 = (src_data_Y + nv21_y1 * src_linebytes_Y)[ixs];
						srcU2 = (src_data_U + nv21_y1/2 * src_linebytes_U)[(ixs>>1)];
						srcV2 = (src_data_V + nv21_y1/2 * src_linebytes_V)[(ixs>>1)];
						
						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];
						
						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						valB = (MInt32)(srcB0*b1 + srcB2*b);
						valG = (MInt32)(srcG0*b1 + srcG2*b);
						valR = (MInt32)(srcR0*b1 + srcR2*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;

	case  PAF_YUYV:
		{
			MByte * src_data_Y = pSrcImgInfo->ppu8Plane[0];
			MByte * src_data_U = pSrcImgInfo->ppu8Plane[1];
			MByte * src_data_V = pSrcImgInfo->ppu8Plane[2];
			MInt32 src_linebytes_Y = pSrcImgInfo->pi32Pitch[0];
			MInt32 src_linebytes_U = pSrcImgInfo->pi32Pitch[1];
			MInt32 src_linebytes_V = pSrcImgInfo->pi32Pitch[2];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					MInt32 nv21_y0, nv21_y1;
					MInt32 srcB0, srcG0, srcR0;
					MInt32 srcB1, srcG1, srcR1;
					MInt32 srcB2, srcG2, srcR2;
					MInt32 srcB3, srcG3, srcR3;
					MInt32 srcY0, srcU0, srcV0;
					MInt32 srcY1, srcU1, srcV1;
					MInt32 srcY2, srcU2, srcV2;
					MInt32 srcY3, srcU3, srcV3;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						nv21_y0 = iys;
						nv21_y1 = nv21_y0 + 1;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						nv21_y0 = nv21_y1 = iys;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;
						MByte * pheader0 = 0;
						MByte * pheader1 = 0;

						pheader0 = src_data_Y + nv21_y0 * src_linebytes_Y + (ixs<<1);
						pheader1 = src_data_Y + nv21_y1 * src_linebytes_Y + (ixs<<1);

						if (ixs%2)
						{
							srcY0 = pheader0[0];
							srcU0 = pheader0[-1];
							srcV0 = pheader0[1];
							
							srcY1 = pheader0[2];
							srcU1 = pheader0[-1];
							srcV1 = pheader0[1];
							
							srcY2 = pheader1[0];
							srcU2 = pheader1[-1];
							srcV2 = pheader1[1];
							
							srcY3 = pheader1[2];
							srcU3 = pheader1[-1];
							srcV3 = pheader1[1];
						}
						else
						{
							srcY0 = pheader0[0];
							srcU0 = pheader0[1];
							srcV0 = pheader0[3];
							
							srcY1 = pheader0[2];
							srcU1 = pheader0[1];
							srcV1 = pheader0[3];
							
							srcY2 = pheader1[0];
							srcU2 = pheader1[1];
							srcV2 = pheader1[3];
							
							srcY3 = pheader1[2];
							srcU3 = pheader1[1];
							srcV3 = pheader1[3];
						}

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];

						srcB1 = srcY1 + table_u_b[srcU1];
						srcG1 = srcY1 + table_u_g[srcU1] + table_v_g[srcV1];
						srcR1 = srcY1 + table_v_r[srcV1];

						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						srcB3 = srcY3 + table_u_b[srcU3];
						srcG3 = srcY3 + table_u_g[srcU3] + table_v_g[srcV3];
						srcR3 = srcY3 + table_v_r[srcV3];
						
						p0 = srcB0*a1 + srcB1*a;
						p1 = srcB2*a1 + srcB3*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = srcG0*a1 + srcG1*a;
						p1 = srcG2*a1 + srcG3*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = srcR0*a1 + srcR1*a;
						p1 = srcR2*a1 + srcR3*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;
						MByte * pheader0 = 0;
						MByte * pheader1 = 0;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						pheader0 = src_data_Y + nv21_y0 * src_linebytes_Y + (ixs<<1);
						pheader1 = src_data_Y + nv21_y1 * src_linebytes_Y + (ixs<<1);

						if (ixs%2)
						{
							srcY0 = pheader0[0];
							srcU0 = pheader0[-1];
							srcV0 = pheader0[1];
							
							srcY2 = pheader1[0];
							srcU2 = pheader1[-1];
							srcV2 = pheader1[1];
						}
						else
						{
							srcY0 = pheader0[0];
							srcU0 = pheader0[1];
							srcV0 = pheader0[3];
							
							srcY2 = pheader1[0];
							srcU2 = pheader1[1];
							srcV2 = pheader1[3];
						}

						srcB0 = srcY0 + table_u_b[srcU0];
						srcG0 = srcY0 + table_u_g[srcU0] + table_v_g[srcV0];
						srcR0 = srcY0 + table_v_r[srcV0];
						
						srcB2 = srcY2 + table_u_b[srcU2];
						srcG2 = srcY2 + table_u_g[srcU2] + table_v_g[srcV2];
						srcR2 = srcY2 + table_v_r[srcV2];

						valB = (MInt32)(srcB0*b1 + srcB2*b);
						valG = (MInt32)(srcG0*b1 + srcG2*b);
						valR = (MInt32)(srcR0*b1 + srcR2*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;

	case PAF_RGB24_B8G8R8:
		{
			MByte * src_data = pSrcImgInfo->ppu8Plane[0];
			MInt32 src_linebytes = pSrcImgInfo->pi32Pitch[0];

			for(y = 0; y < dst_height; y++, dst_data += dst_linebytes)
			{
				double xs = param3 * y + f1;
				double ys = param4 * y + f2;
				
				for (x = 0; x < dst_width; x++)
				{
					MInt32 ixs = (MInt32)(floor(xs));
					MInt32 iys = (MInt32)(floor(ys));
					double a = xs - ixs, b = ys - iys;
					const MByte *ptr0, *ptr1;
					
					xs += param1;
					ys += param2;
					
					if (iys >= 0 && iys < src_height-1)
					{
						ptr0 = src_data + src_linebytes*iys;
						ptr1 = ptr0 + src_linebytes;
					}
					else
					{
						iys = (iys < 0) ? 0 : (src_height-1);
						ptr0 = ptr1 = src_data + iys*src_linebytes;
					}
					
					if (ixs >= 0 && ixs < src_width-1)
					{
						double a1 = 1.f - a;
						double p0, p1;
						
						ptr0 += ixs*3;
						ptr1 += ixs*3;
						
						p0 = ptr0[0]*a1 + ptr0[3]*a;
						p1 = ptr1[0]*a1 + ptr1[3]*a;
						valB = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						
						p0 = ptr0[1]*a1 + ptr0[4]*a;
						p1 = ptr1[1]*a1 + ptr1[4]*a;
						valG = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						
						p0 = ptr0[2]*a1 + ptr0[5]*a;
						p1 = ptr1[2]*a1 + ptr1[5]*a;
						valR = (MInt32)(p0 + b * (p1 - p0));
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
					else
					{
						double b1 = 1.f - b;

						ixs = (ixs < 0) ? 0 : (src_width-1);
						
						ptr0 += ixs*3;
						ptr1 += ixs*3;
						
						valB = (MInt32)(ptr0[0]*b1 + ptr1[0]*b);
						valG = (MInt32)(ptr0[1]*b1 + ptr1[1]*b);
						valR = (MInt32)(ptr0[2]*b1 + ptr1[2]*b);
						
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3] = CAST_8U(valB);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+1] = CAST_8U(valG);
						dst_data[((tran^0)*(dst_width-x) + (tran^1)*x)*3+2] = CAST_8U(valR);
					}
				}
			}
		}
		break;
	}

	if (p_param)
	{
		p_param[0] = param1;
		p_param[1] = param2;
		p_param[2] = param3;
		p_param[3] = param4;
		p_param[4] = f1;
		p_param[5] = f2;
	}

	return 0;
}
