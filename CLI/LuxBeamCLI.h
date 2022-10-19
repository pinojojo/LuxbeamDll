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
	/// ����UDP�׽�������
	/// </summary>
	/// <returns></returns>
	int Connect();

	/// <summary>
	/// ȡ�����ӣ���ȫ�ػ�
	/// </summary>
	/// <returns></returns>
	int DisConnect();

	/// <summary>
	/// ���������һ��Reply�Ƿ�OK
	/// </summary>
	/// <returns></returns>
	int CheckLastReply();

	/// <summary>
	/// ��Connect��������֮���ٵ���һ��
	/// </summary>
	/// <returns></returns>
	int SetImageType();

	/// <summary>
	/// ���õ�ǰ�ϴ�ͼ�������
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	int SetImageSize(int val);

	/// <summary>
	/// ���ù���ģʽ 0�������ģʽ 1�����ع�һ���ض�ʱ��
	/// </summary>
	/// <param name="mode"></param>
	/// <returns></returns>
	int SetWorkingMode(int mode);

	/// <summary>
	/// �����ϴ�Sequence�ļ�
	/// </summary>
	/// <returns></returns>
	int ReLoadSequenceFile();

	/// <summary>
	/// ����DMD�ع�ʱ��
	/// </summary>
	/// <param name="val"></param>
	/// <returns></returns>
	int SetHoldingTime(float val);

	/// <summary>
	/// �����ű�
	/// </summary>
	/// <returns></returns>
	int StartSequence();

	/// <summary>
	/// ֹͣ�ű�
	/// </summary>
	/// <returns></returns>
	int StopSequence();

	/// <summary>
	/// ���ýű�
	/// </summary>
	/// <returns></returns>
	int ResetSequence();

	/// <summary>
	/// �ű��������״̬
	/// </summary>
	/// <returns></returns>
	int UnResetSequence();

	/// <summary>
	/// �������ü��������������������Ǽ��������ͼƬ���ݰ��Ƿ�������
	/// </summary>
	/// <returns></returns>
	int ResetSequenceNumberCounter();

	/// <summary>
	/// ���ļ�·���ж�ȡһ��ͼƬ
	/// </summary>
	/// <param name="imgPath"> ·�� </param>
	/// <param name="inum"> �����ڴ�λ�� </param>
	/// <returns></returns>
	int LoadImageFromFile(String^ imgPath, int inum);

	/// <summary>
	/// ��C#��Byte Array ��ȡ����
	/// </summary>
	/// <param name="data"></param>
	/// <param name="size"></param>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadImageFromBytes(void* data, int size, int inum);

	/// <summary>
	/// �ϴ�����ͼƬ
	/// </summary>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadWhite(int inum);

	/// <summary>
	/// �ϴ�����ͼƬ
	/// </summary>
	/// <param name="inum"></param>
	/// <returns></returns>
	int LoadBlack(int inum);










};

