#pragma once

#define CRACKPROCESSDLL_API __declspec(dllexport)

/* 检测裂缝 */
extern "C" CRACKPROCESSDLL_API int __stdcall FindCracks(char* srcFileName, char* dstFileName, int red, int green, int blue);

/* 滤波 */
extern "C" CRACKPROCESSDLL_API int __stdcall Filter(char* srcFileName, char* dstFileName, int red, int green, int blue);

/* 测量 */
extern "C" CRACKPROCESSDLL_API int __stdcall MeasureCracks(char* srcFileName, char* dstFileName, int red, int green, int blue);

/* 拼接图片 */
extern "C" CRACKPROCESSDLL_API int __stdcall Stitcher(char* srcFileNames, char* dstFileName);