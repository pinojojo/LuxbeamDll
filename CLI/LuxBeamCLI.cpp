#include "LuxBeamCLI.h"
#include <vector>

LuxBeamCLI::LuxBeamCLI()
{

}

int LuxBeamCLI::Connect()
{
	return ConnectNative();
}

// ȡ������ ����LuxBeam���Ӱ�ȫ�ػ�
int LuxBeamCLI::DisConnect()
{
	return DisConnectNative();
}

// ���������һ��Reply�Ƿ�OK
int LuxBeamCLI::CheckLastReply()
{
	return CheckLastReplyNative();
}

// ÿ�γ�ʼ����ʱ�����һ�Σ����ߴ���ȥ��ͼƬ�����Ƕ����Ƶģ��Ҳ�ѹ�����˺���ֻ����һ�Σ�֮��Ͳ���Ҫ��
int LuxBeamCLI::SetImageType()
{
	return SetImageTypeNative();
}

// ����ͼƬ��С����ʵ�����������ڸ�ӡ��ģʽ��һ��val����ֵ��1080
int LuxBeamCLI::SetImageSize(int val)
{
	return SetHoldingTimeNative(val);
}

// ÿ��׼����ʾͼƬǰ��
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
	// C#��C++������ת�� ��String��const char*
	// �ο� https://docs.microsoft.com/en-us/troubleshoot/developer/visualstudio/csharp/language-compilers/convert-systemstring-char#ccli-sample-code-visual-c-2005-and-visual-c-2008
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
