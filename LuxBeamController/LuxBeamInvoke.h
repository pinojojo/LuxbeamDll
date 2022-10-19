#pragma once

#include <stdio.h>
#include <stdlib.h>

#define LUX_API __declspec(dllexport)

// 连接和断开
extern "C" LUX_API int Connect();
extern "C" LUX_API int DisConnect();

// 工作模式
extern "C" LUX_API int SetWorkingMode(int mode);

// 图像上传
extern "C" LUX_API int LoadImage(unsigned char* data, int size, int inum);
extern "C" LUX_API int LoadLongCompactImage(unsigned char* data, int size, int inum, int rows);

// 触发
extern "C" LUX_API int GetTriggerInfo(int& pe, int& po, int& pere, int& pero, int& pseq);
extern "C" LUX_API int SetTriggerMode(int trigMode);

// 脚本操作
extern "C" LUX_API int SetExposure(float exp);
extern "C" LUX_API int StartSequence();
extern "C" LUX_API int StopSequence();
extern "C" LUX_API int ResetSequence();
extern "C" LUX_API int UnResetSequence();

// 工具函数
extern "C" LUX_API int KeepDisplay(int val);
extern "C" LUX_API int ResetAll();
