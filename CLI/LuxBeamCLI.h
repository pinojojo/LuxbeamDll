#pragma once
#include <atlstr.h>
#include <stdio.h>
#using <mscorlib.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

#include <msclr/marshal.h>
using namespace msclr::interop;

#include <iostream>
#include "../LuxBeamController/LuxBeamInvoke.h"

public ref class LuxBeamCLI
{
public:
	LuxBeamCLI();

	/// <summary> 
	/// 建立UDP套接字连接
	/// </summary>
	/// <returns></returns>
	int Connect();

	/// <summary>
	/// 取消连接，安全关机
	/// </summary>
	/// <returns></returns>
	int DisConnect();

	/// <summary>
	/// 检验最近的一次Reply是否OK
	/// </summary>
	/// <returns></returns>
	int CheckLastReply();

	/// <summary>
	/// 在Connect函数调用之后，再调用一次
	/// </summary>
	/// <returns></returns>
	int SetImageType();

	/// <summary>
	/// 设置当前上传图像的列数
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	int SetImageSize(int val);

	/// <summary>
	/// 设置工作模式 0代表持续模式 1代表曝光一个特定时长
	/// </summary>
	/// <param name="mode"></param>
	/// <returns></returns>
	int SetWorkingMode(int mode);

	/// <summary>
	/// 重新上传Sequence文件
	/// </summary>
	/// <returns></returns>
	int ReLoadSequenceFile();

	/// <summary>
	/// 设置DMD曝光时间
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	int SetHoldingTime(float val);

	/// <summary>
	/// 启动脚本
	/// </summary>
	/// <returns></returns>
	int StartSequence();

	/// <summary>
	/// 停止脚本
	/// </summary>
	/// <returns></returns>
	int StopSequence();

	/// <summary>
	/// 重置脚本
	/// </summary>
	/// <returns></returns>
	int ResetSequence();

	/// <summary>
	/// 脚本解除重置状态
	/// </summary>
	/// <returns></returns>
	int UnResetSequence();

	/// <summary>
	/// 重新设置计数器，计数器的作用是计数传入的图片数据包是否发生丢包
	/// </summary>
	/// <returns></returns>
	int ResetSequenceNumberCounter();

	/// <summary>
	/// 从文件路径中读取一张图片
	/// </summary>
	/// <param name="imgPath"> 路径 </param>
	/// <param name="inum"> 板载内存位置 </param>
	/// <returns></returns>
	int LoadImageFromFile(String^ imgPath, int inum);

	/// <summary>
	/// 从C#的Byte Array 读取数据
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadImageFromBytes(void* data, int size, int inum);

	/// <summary>
	/// 上传纯白图片
	/// </summary>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadWhite(int inum);

	/// <summary>
	/// 上传纯黑图片
	/// </summary>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadBlack(int inum);










};

