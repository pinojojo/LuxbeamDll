#pragma once

#include "UDPSender.h"
#include "UDPReceiver.h"
#include "LuxImage.h"
#include "Sequencer.h"

struct Packet
{
	void Append(std::vector<unsigned char> bytes) {
		Record.insert(Record.end(), bytes.begin(), bytes.end());
	}

	void Print() {
		std::cout << "Packet: ";
		for (size_t i = 0; i < Record.size(); i++)
		{
			std::cout << (int)(Record[i]) << " ";
		}
		std::cout << std::endl;
	}

	std::vector<unsigned char> Record;
};

class LuxBeamController
{
public:
	LuxBeamController(const LuxBeamController&) = delete;
	static LuxBeamController& GetInstance() {
		static LuxBeamController instance;
		return instance;
	}
	// �豸״̬
	bool Connect();
	void Shutdown();

	// �ⴥ�����
	void SetTriggerMode();
	void GetTriggerInfo(int& p_e, int& p_o, int& per_e, int& per_o, int& p_seq);

	// ����DMD����ģʽ������ �� �ع��ض�ʱ��
	void SetWorkingMode(int mode);

	// �ú���ͨ���ڸı��ع�ʱ��֮�� ��������Sequence�ļ�
	void ResetWorkingMode();

	void SetImageType(int type);
	void SetImageSize(int rows);

	// Sequnce�ļ�����
	void LoadSequenceFile(std::string seqFilePath);
	void LoadWorkingSequenceFile();
	void SetImageHolidingTime(int millis);
	void UseInternalSequenceFile();

	// �ϴ�����
	void UploadTemplateImage();
	void UploadBlackImageToInum1();
	void UploadBlackImageToInum0();
	void UploadWhiteImageToInum1();
	void UploadWhiteImageToInum0();
	void UploadImage(const char* path);
	void UploadImageFromFileToPosition(const char* path, int inum);
	void UploadImageFromBytesToPosition(unsigned char* data, int size, int inum);
	void UploadLongImageFromBytesToPosition(unsigned char* data, int size, int inum); // �ϴ���ͼ

	void SetTriggerMode(int mode);

	// ÿ���ϴ�ͼƬPacket֮ǰ����Ҫ���ô˺������ü�����
	void ResetSequenceNumberCounter();
	void ResetSequence();
	void UnResetSequence();
	void StopSequence();
	void StartSequence();

	void RequireSequenceState();

	// Test Image
	void SetTemplate(int width, int cycle);
	void SetTemplate(int width, int cycle, int fovWidth, int fovHeight);
	void ReloadTemplateAndZero();
	void SetTemplatePatternType(int type);

	bool CheckReplayOK(std::vector<unsigned char> reply);
	bool CheckReply();

private:
	LuxBeamController();

	LuxImage				 m_SampleImage;
	LuxImage				 m_ZeroImage;
	Sequencer				 m_Sequencer;

	DMDUtils::UDPSender		 m_UdpSender;
	DMDUtils::UDPReceiver	 m_UdpReceiver;

	std::vector<unsigned char> m_LuxBeamReplyOK;
	std::vector<unsigned char> m_LuxBeamReply;  //���һ�ε�Reply

	bool					 m_Initialized = false;
	int						 m_WorkingMode = 0; // 0-����ģʽ  1-�ع�ģʽ
};
