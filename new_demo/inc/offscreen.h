#ifndef __OFFSCREEN_H__
#define __OFFSCREEN_H__

#include "comdef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EVEN(x) (x&0xfffffffe)
//	31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00 

//													 R  R  R  R  R  G  G  G  G  G  G  B  B  B  B  B 
#define		PAF_RGB16_B5G6R5		0x101
//													 X  R  R  R  R  R  G  G  G  G  G  B  B  B  B  B  
#define		PAF_RGB16_B5G5R5		0x102
//													 X  X  X  X  R  R  R  R  G  G  G  G  B  B  B  B 
#define		PAF_RGB16_B4G4R4		0x103
//													 T  R  R  R  R  R  G  G  G  G  G  B  B  B  B  B  
#define		PAF_RGB16_B5G5R5T		0x104
//													 B  B  B  B  B  G  G  G  G  G  G  R  R  R  R  R  
#define		PAF_RGB16_R5G6B5		0x105
//													 X  B  B  B  B  B  G  G  G  G  G  R  R  R  R  R   
#define		PAF_RGB16_R5G5B5		0x106
//													 X  X  X  X  B  B  B  B  G  G  G  G  R  R  R  R  
#define		PAF_RGB16_R4G4B4		0x107


//							 R	R  R  R	 R	R  R  R  G  G  G  G  G  G  G  G  B  B  B  B  B  B  B  B 
#define		PAF_RGB24_B8G8R8		0x201
//							 X	X  X  X	 X	X  R  R  R  R  R  R  G  G  G  G  G  G  B  B  B  B  B  B  
#define		PAF_RGB24_B6G6R6		0x202
//							 X	X  X  X	 X	T  R  R  R  R  R  R  G  G  G  G  G  G  B  B  B  B  B  B  
#define		PAF_RGB24_B6G6R6T		0x203
//							 B  B  B  B  B  B  B  B  G  G  G  G  G  G  G  G  R	R  R  R	 R	R  R  R 
#define		PAF_RGB24_R8G8B8		0x204
//							 X	X  X  X	 X	X  B  B  B  B  B  B  G  G  G  G  G  G  R  R  R  R  R  R 
#define		PAF_RGB24_R6G6B6		0x205

//	 X	X  X  X	 X	X  X  X	 R	R  R  R	 R	R  R  R  G  G  G  G  G  G  G  G  B  B  B  B  B  B  B  B  
#define		PAF_RGB32_B8G8R8		0x301
//	 A	A  A  A	 A	A  A  A	 R	R  R  R	 R	R  R  R  G  G  G  G  G  G  G  G  B  B  B  B  B  B  B  B  
#define		PAF_RGB32_B8G8R8A8		0x302
//	 X	X  X  X	 X	X  X  X	 B  B  B  B  B  B  B  B  G  G  G  G  G  G  G  G  R	R  R  R	 R	R  R  R  
#define		PAF_RGB32_R8G8B8		0x303
//	 B  B  B  B  B  B  B  B  G  G  G  G  G  G  G  G  R	R  R  R	 R	R  R  R  A	A  A  A	 A	A  A  A
#define		PAF_RGB32_A8R8G8B8		0x304

//			Y0, U0, V0																				
#define		PAF_YUV				0x401
//			Y0, V0, U0																				
#define		PAF_YVU				0x402
//			U0, V0, Y0																				
#define		PAF_UVY				0x403
//			V0, U0, Y0																				
#define		PAF_VUY				0x404

//			Y0, U0, Y1, V0																			
#define		PAF_YUYV				0x501
//			Y0, V0, Y1, U0																			
#define		PAF_YVYU				0x502
//			U0, Y0, V0, Y1																			
#define		PAF_UYVY				0x503
//			V0, Y0, U0, Y1																			
#define		PAF_VYUY				0x504
//			Y1, U0, Y0, V0																			
#define		PAF_YUYV2				0x505
//			Y1, V0, Y0, U0																			
#define		PAF_YVYU2				0x506
//			U0, Y1, V0, Y0																			
#define		PAF_UYVY2				0x507
//			V0, Y1, U0, Y0																			
#define		PAF_VYUY2				0x508

//8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
#define		PAF_I420				0x601
//8 bit Y plane followed by 8 bit 1x2 subsampled U and V planes
#define		PAF_I422V				0x602
//8 bit Y plane followed by 8 bit 2x1 subsampled U and V planes
#define		PAF_I422H				0x603
//8 bit Y plane followed by 8 bit U and V planes
#define		PAF_I444				0x604
//8 bit Y plane followed by 8 bit 2x2 subsampled V and U planes
#define		PAF_YV12				0x605
//8 bit Y plane followed by 8 bit 1x2 subsampled V and U planes	
#define		PAF_YV16V				0x606
//8 bit Y plane followed by 8 bit 2x1 subsampled V and U planes
#define		PAF_YV16H				0x607
//8 bit Y plane followed by 8 bit V and U planes
#define		PAF_YV24				0x608
//8 bit Y plane only
#define		PAF_GRAY				0x701


//8 bit Y plane followed by 8 bit 2x2 subsampled UV planes
#define		PAF_NV12				0x801
//8 bit Y plane followed by 8 bit 2x2 subsampled VU planes
#define		PAF_NV21				0x802
//8 bit Y plane followed by 8 bit 2x1 subsampled UV planes
#define		PAF_LPI422H			0x803


//Negative UYVY, U0, Y0, V0, Y1																			
#define		PAF_NEG_UYVY			0x901
//Negative I420, 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
#define		PAF_NEG_I420			0x902


//Mono UYVY, UV values are fixed, gray image in U0, Y0, V0, Y1
#define		PAF_MONO_UYVY			0xa01
//Mono I420, UV values are fixed, 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
#define		PAF_MONO_I420			0xa02

//P8_YUYV, 8 pixels a group, Y0Y1Y2Y3Y4Y5Y6Y7U0U1U2U3V0V1V2V3
#define		PAF_P8_YUYV			0xb03

typedef struct __tag_OFFSCREEN
{
	MUInt32	u32PixelArrayFormat;
	MInt32	i32Width;
	MInt32	i32Height;
	MUInt8*	ppu8Plane[4];
	MInt32	pi32Pitch[4];
}OFFSCREEN, *LPOFFSCREEN;

#ifdef __cplusplus
}
#endif

#endif //__OFFSCREEN_H__
