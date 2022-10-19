#include "LuxBeamCLI.h"
#include <vector>

LuxBeamCLI::LuxBeamCLI()
{

}

int LuxBeamCLI::Connect()
{
	return ConnectNative();
}

// 取消连接 告诉LuxBeam板子安全关机
int LuxBeamCLI::DisConnect()
{
	return DisConnectNative();
}

// 检验最近的一次Reply是否OK
int LuxBeamCLI::CheckLastReply()
{
	return CheckLastReplyNative();
}

// 每次初始化的时候调用一次，告诉传进去的图片数据是二进制的，且不压缩。此函数只调用一次，之后就不需要了
int LuxBeamCLI::SetImageType()
{
	return SetImageTypeNative();
}

// 设置图片大小，其实是行数，现在盖印章模式，一般val传数值是1080
int LuxBeamCLI::SetImageSize(int val)
{
	return SetHoldingTimeNative(val);
}

// 每次准备显示图片前，
int LuxBeamCLI::SetWorkingMode(int mode)
{
	return SetWorkingModeNative(mode);
}

int LuxBeamCLI::ReLoadSequenceFile()
{
	return ReLoadSequenceFileNative();
}



int LuxBeamCLI::SetHoldingTime(float val)
{
	return SetHoldingTimeNative(val);
}

int LuxBeamCLI::StartSequence()
{
	return StartSequenceNative();
}

int LuxBeamCLI::StopSequence()
{
	return StopSequenceNative();
}

int LuxBeamCLI::ResetSequence()
{
	return ResetSequenceNative();
}

int LuxBeamCLI::UnResetSequence()
{
	return UnResetSequenceNative();
}

int LuxBeamCLI::ResetSequenceNumberCounter()
{
	return ResetSequenceNumberCounterNative();
}

int LuxBeamCLI::LoadImageFromFile(String^ imgPath, int inum)
{
	// C#到C++的类型转换 从String到const char*
	// 参考 https://docs.microsoft.com/en-us/troubleshoot/developer/visualstudio/csharp/language-compilers/convert-systemstring-char#ccli-sample-code-visual-c-2005-and-visual-c-2008
	marshal_context^ context = gcnew marshal_context();
	const char* str = context->marshal_as<const char*>(imgPath);
	return LoadImageFromFileNative(str, inum);
}


int LuxBeamCLI::LoadImageFromBytes(void* data, int size, int inum)
{
	unsigned char* memPtr = reinterpret_cast<unsigned char*>(data);

	unsigned char temp[1920*1080];

	int ret=LoadImageNative(temp, size, inum);
	
	return ret;
}

int LuxBeamCLI::LoadWhite(int inum)
{
	return LoadWhiteNative(inum);
}

int LuxBeamCLI::LoadBlack(int inum)
{
	return LoadBlackNative(inum);
}
