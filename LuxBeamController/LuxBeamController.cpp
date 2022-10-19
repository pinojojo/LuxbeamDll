#include "LuxBeamController.h"
#include "LuxHelper.h"
LuxBeamController::LuxBeamController()
{
	// Variables
	m_LuxBeamReplyOK.clear();

	std::vector<unsigned char> bytes;
	bytes = lux::ConvertIntToDualBytes(6);
	m_LuxBeamReplyOK.insert(m_LuxBeamReplyOK.end(), bytes.begin(), bytes.end());
	bytes = lux::ConvertIntToDualBytes(501);
	m_LuxBeamReplyOK.insert(m_LuxBeamReplyOK.end(), bytes.begin(), bytes.end());
	bytes = lux::ConvertIntToDualBytes(0);
	m_LuxBeamReplyOK.insert(m_LuxBeamReplyOK.end(), bytes.begin(), bytes.end());
}

bool LuxBeamController::Connect()
{
	lux::LogToFile("connect");
	// Some var init
	//m_TextureTemplateShow.Create(DMD_WIDTH, DMD_HEIGHT);

	// Create UDP
	if (!m_Initialized)
	{
		lux::LogToFile("connect::init");

		bool initStatus = true;
		initStatus &= m_UdpSender.Init("192.168.0.10", 52985, "192.169.0.20", 30000);

		if (initStatus)
		{
			m_Initialized = true;
		}

		return initStatus;
	}
	else
	{
		std::cout << "UDP Socket Created Once!." << std::endl;

		return true;
	}

	return true;
}

void LuxBeamController::Shutdown()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(4));
	pack.Append(lux::ConvertIntToDualBytes(180));

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply());
}

void LuxBeamController::SetWorkingMode(int mode)
{
	m_WorkingMode = mode;
	if (mode == 0)
	{
		UseInternalSequenceFile();
	}
	if (mode == 1)
	{
		LoadWorkingSequenceFile();
	}
	if (mode == 2)
	{
		// 此时将上传灰度模式脚本
	}
}

void LuxBeamController::ResetWorkingMode()
{
	SetWorkingMode(m_WorkingMode);
}

void LuxBeamController::SetImageType(int type)
{
	if (type == 0)
	{
		Packet pack;

		pack.Append(lux::ConvertIntToDualBytes(6));//size
		pack.Append(lux::ConvertIntToDualBytes(103));//id
		pack.Append(lux::ConvertIntToDualBytes(1));//type 1:uncompressed binary

		m_UdpSender.Send(pack.Record.data(), pack.Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}
}

void LuxBeamController::SetImageSize(int rows)
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(6));//size
	pack.Append(lux::ConvertIntToDualBytes(102));//id
	pack.Append(lux::ConvertIntToDualBytes(rows));//size

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::LoadSequenceFile(std::string seqFilePath)
{
	m_Sequencer.LoadFromFile(seqFilePath);

	m_UdpSender.Send(m_Sequencer.m_Record.data(), m_Sequencer.m_Record.size());

	CheckReplayOK(m_UdpSender.GetReply());
}

void LuxBeamController::LoadWorkingSequenceFile()
{
	std::string seqFilePath = "assets/sequence/working.txt";
	LoadSequenceFile(seqFilePath);
}

void LuxBeamController::SetImageHolidingTime(int millis)
{
	std::string seqFilePath = "assets/sequence/seq_keep_a_time_ms.txt";

	std::ifstream seqFile;

	seqFile.open(seqFilePath);
	std::string contents((std::istreambuf_iterator<char>(seqFile)),
		std::istreambuf_iterator<char>());
	seqFile.close();

	auto value = lux::ReplaceBetween(contents, "AssignVar NeedToJumpBlack ", " ", std::to_string(millis));

	std::cout << value << std::endl;

	std::string savingFilePath = "assets/sequence/working.txt";
	if (std::filesystem::exists(savingFilePath))
	{
		std::remove(savingFilePath.c_str());
		//then
		std::ofstream outfile(savingFilePath.c_str());
		outfile << value;
		outfile.close();
	}
	else
	{
		std::ofstream outfile(savingFilePath.c_str());
		outfile << value;
		outfile.close();
	}

	// Reload Sequence File
	LoadWorkingSequenceFile();
}

void LuxBeamController::UseInternalSequenceFile()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(5));//size
	pack.Append(lux::ConvertIntToDualBytes(171));//id
	pack.Append(lux::ConvertIntToSingleBytes(1));//size

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply());
}

void LuxBeamController::UploadTemplateImage()
{
	m_SampleImage.Clear();
	m_SampleImage.GenerateTemplate();

	m_SampleImage.MakePackets(0);

	SetImageType(0);

	SetImageSize(m_SampleImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_SampleImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_SampleImage.m_Packets[i].Record.data(), m_SampleImage.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}

	std::cout << "Template Image Uploaded." << std::endl;
}

void LuxBeamController::UploadBlackImageToInum1()
{
	m_ZeroImage.MakeZeroImage(false);

	m_ZeroImage.MakePackets(1);

	SetImageType(0);

	SetImageSize(m_ZeroImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_ZeroImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_ZeroImage.m_Packets[i].Record.data(), m_ZeroImage.m_Packets[i].Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}

	std::cout << "Black Image Uploaded." << std::endl;
}

void LuxBeamController::UploadBlackImageToInum0()
{
	m_ZeroImage.MakeZeroImage(false);

	m_ZeroImage.MakePackets(0);

	SetImageType(0);

	SetImageSize(m_ZeroImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_ZeroImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_ZeroImage.m_Packets[i].Record.data(), m_ZeroImage.m_Packets[i].Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}

	std::cout << "Black Image Uploaded." << std::endl;
}

void LuxBeamController::UploadWhiteImageToInum1()
{
	LuxImage image;
	image.MakeZeroImage(true);

	image.MakePackets(1);

	SetImageType(0);

	SetImageSize(image.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < image.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(image.m_Packets[i].Record.data(), image.m_Packets[i].Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}

	std::cout << "wite Image Uploaded." << std::endl;
}

void LuxBeamController::UploadWhiteImageToInum0()
{
	LuxImage image;
	image.MakeZeroImage(true);

	image.MakePackets(0);

	SetImageType(0);

	SetImageSize(image.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < image.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(image.m_Packets[i].Record.data(), image.m_Packets[i].Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}

	std::cout << "wite Image Uploaded." << std::endl;
}

void LuxBeamController::UploadImage(const char* path)
{
	LuxImage luxImg;

	luxImg.LoadFromBmpFile(path);

	luxImg.MakePackets(0);

	SetImageType(0);

	SetImageSize(luxImg.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < luxImg.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(luxImg.m_Packets[i].Record.data(), luxImg.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}
	std::cout << "Loaded BMP File: " << path << std::endl;
}

void LuxBeamController::UploadImageFromFileToPosition(const char* path, int inum)
{
	LuxImage luxImg;

	// 读取图片
	luxImg.LoadFromBmpFile(path);

	// 设置上传内存位置
	luxImg.MakePackets(inum);

	SetImageType(0);

	SetImageSize(luxImg.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < luxImg.m_Packets.size(); i++)
	{
		m_UdpSender.Send(luxImg.m_Packets[i].Record.data(), luxImg.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}
	std::cout << "Loaded BMP File: " << path << std::endl;
}

void LuxBeamController::UploadImageFromBytesToPosition(unsigned char* data, int size, int inum)
{
	LuxImage luxImg;

	luxImg.Clear();

	std::vector<unsigned char> memcpp;
	memcpp.resize(1920 * 1080);

	for (size_t i = 0; i < 1920 * 1080; i++)
	{
		memcpp[i] = data[i];
	}

	// 从内存指针开始读取图片
	luxImg.LoadFromBytes(memcpp.data(), size);

	// 设置上传目标内存位置
	luxImg.MakePackets(inum);

	SetImageType(0);

	SetImageSize(luxImg.m_Rows);

	lux::LogToFile("img rows: " + std::to_string(luxImg.m_Rows));
	lux::LogToFile("img cols: " + std::to_string(luxImg.m_Cols));

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < luxImg.m_Packets.size(); i++)
	{
		m_UdpSender.Send(luxImg.m_Packets[i].Record.data(), luxImg.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}
}

void LuxBeamController::UploadLongImageFromBytesToPosition(unsigned char* data, int size, int inum)
{
	LuxImage luxImg;

	luxImg.LoadFromCompactBytes(data, size);
	luxImg.MakePackets(inum);

	SetImageType(0);
	SetImageSize(luxImg.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < luxImg.m_Packets.size(); i++)
	{
		m_UdpSender.Send(luxImg.m_Packets[i].Record.data(), luxImg.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}
}

void LuxBeamController::SetTriggerMode(int mode)
{
	if (mode == 0)
	{
		// Internal
		//donothing
	}
	if (mode == 1)
	{
		Packet pack;

		pack.Append(lux::ConvertIntToDualBytes(8));
		pack.Append(lux::ConvertIntToDualBytes(160));
		pack.Append(lux::ConvertIntToDualBytes(0));//electrical io
		pack.Append(lux::ConvertIntToDualBytes(0));//0: positive edge 1: negative edge

		m_UdpSender.Send(pack.Record.data(), pack.Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}
}

void LuxBeamController::SetTriggerMode()
{
	// 调用此函数将默认 电信号 在上升沿 触发
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(8));
	pack.Append(lux::ConvertIntToDualBytes(160));
	pack.Append(lux::ConvertIntToDualBytes(0));
	pack.Append(lux::ConvertIntToDualBytes(0));

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check

	//
}

void LuxBeamController::GetTriggerInfo(int& p_e, int& p_o, int& per_e, int& per_o, int& p_seq)
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(4));
	pack.Append(lux::ConvertIntToDualBytes(352));

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	std::vector<unsigned char> reply = m_UdpSender.GetReply();

	// 解析返回查询值
	if (reply.size() == 24)
	{
		lux::LogToFile("trigger info requested");

		{
			std::vector<unsigned char> bytes(reply.begin() + 4, reply.begin() + 8);
			p_o = lux::ConvertQuadBytesToInt(bytes);
		}
		{
			std::vector<unsigned char> bytes(reply.begin() + 8, reply.begin() + 14);
			p_e = lux::ConvertQuadBytesToInt(bytes);
		}
		{
			std::vector<unsigned char> bytes(reply.begin() + 12, reply.begin() + 14);
			per_o = lux::ConvertDualBytesToInt(bytes);
		}
		{
			std::vector<unsigned char> bytes(reply.begin() + 14, reply.begin() + 16);
			per_e = lux::ConvertDualBytesToInt(bytes);
		}
		{
			std::vector<unsigned char> bytes(reply.begin() + 16, reply.begin() + 20);
			p_seq = lux::ConvertQuadBytesToInt(bytes);
		}
	}
}

void LuxBeamController::ResetSequenceNumberCounter()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(4));
	pack.Append(lux::ConvertIntToDualBytes(112));

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::ResetSequence()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(6));
	pack.Append(lux::ConvertIntToDualBytes(106)); // set sequence state
	pack.Append(lux::ConvertIntToSingleBytes(2)); // set sequence state: reset
	pack.Append(lux::ConvertIntToSingleBytes(1)); // set sequence state

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::UnResetSequence()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(6));
	pack.Append(lux::ConvertIntToDualBytes(106)); // set sequence state
	pack.Append(lux::ConvertIntToSingleBytes(2)); // set sequence state: reset
	pack.Append(lux::ConvertIntToSingleBytes(0)); // set sequence state

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::StopSequence()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(6));
	pack.Append(lux::ConvertIntToDualBytes(106)); // set sequence state
	pack.Append(lux::ConvertIntToSingleBytes(1)); // set sequence state: stop
	pack.Append(lux::ConvertIntToSingleBytes(0)); // set sequence state

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::StartSequence()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(6));
	pack.Append(lux::ConvertIntToDualBytes(106)); // set sequence state
	pack.Append(lux::ConvertIntToSingleBytes(1)); // set sequence state: start
	pack.Append(lux::ConvertIntToSingleBytes(1)); // set sequence state

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::RequireSequenceState()
{
	Packet pack;

	pack.Append(lux::ConvertIntToDualBytes(5));
	pack.Append(lux::ConvertIntToDualBytes(306)); // set sequence state
	pack.Append(lux::ConvertIntToSingleBytes(1));

	m_UdpSender.Send(pack.Record.data(), pack.Record.size());

	CheckReplayOK(m_UdpSender.GetReply()); //check
}

void LuxBeamController::SetTemplate(int width, int cycle)
{
	m_SampleImage.SetTemplate(width, cycle);
	m_SampleImage.MakePackets(0);

	SetImageType(0);

	SetImageSize(m_SampleImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_SampleImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_SampleImage.m_Packets[i].Record.data(), m_SampleImage.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}

	std::cout << "Template Image Uploaded." << std::endl;
}

void LuxBeamController::SetTemplate(int width, int cycle, int fovWidth, int fovHeight)
{
	// 更新上传图片数据
	m_SampleImage.SetTemplate(width, cycle, fovWidth, fovHeight);
	m_SampleImage.MakePackets(0); //分包
	SetImageType(0);
	SetImageSize(m_SampleImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_SampleImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_SampleImage.m_Packets[i].Record.data(), m_SampleImage.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}

	// 更新测试图预览纹理数据
	//unsigned char* textureData = m_SampleImage.GetRGBViewable();
	//m_TextureTemplateShow.Update(textureData);

	std::cout << "Template Image Uploaded." << std::endl;
}

void LuxBeamController::ReloadTemplateAndZero()
{
	// 重新上传测试图
	SetImageType(0);
	SetImageSize(m_SampleImage.m_Rows);

	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_SampleImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_SampleImage.m_Packets[i].Record.data(), m_SampleImage.m_Packets[i].Record.size());
		CheckReplayOK(m_UdpSender.GetReply());
	}

	// 重新上传黑图
	SetImageSize(m_ZeroImage.m_Rows);
	ResetSequenceNumberCounter();

	for (size_t i = 0; i < m_ZeroImage.m_Packets.size(); i++)
	{
		//std::cout << "Pack : " << i << std::endl;
		m_UdpSender.Send(m_ZeroImage.m_Packets[i].Record.data(), m_ZeroImage.m_Packets[i].Record.size());

		CheckReplayOK(m_UdpSender.GetReply()); //check
	}
}

void LuxBeamController::SetTemplatePatternType(int type)
{
	m_SampleImage.SetTemplateType(type);
}

bool LuxBeamController::CheckReplayOK(std::vector<unsigned char> reply)
{
	m_LuxBeamReply = reply;

	bool SameAsOK = true;
	if (reply.size() == 6)
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (reply[i] != m_LuxBeamReplyOK[i])
			{
				SameAsOK = false;
			}
		}
	}
	else
	{
		SameAsOK = false;
	}

	if (SameAsOK)
	{
		return true;
	}
	else
	{
		std::cout << "LuxBeam Error: Reply is not OK" << std::endl;
		std::cout << "Error Reply: " << std::endl;
		for (size_t i = 0; i < reply.size(); i++)
		{
			std::cout << (int)reply[i] << " ";
		}
		std::cout << std::endl;
	}
}

bool LuxBeamController::CheckReply()
{
	bool SameAsOK = true;
	if (m_LuxBeamReply.size() == 6)
	{
		for (size_t i = 0; i < 6; i++)
		{
			if (m_LuxBeamReply[i] != m_LuxBeamReplyOK[i])
			{
				SameAsOK = false;
			}
		}
	}
	else
	{
		SameAsOK = false;
	}

	if (SameAsOK)
	{
		return true;
	}
	else
	{
		std::cout << "LuxBeam Error: Reply is not OK" << std::endl;
		std::cout << "Error Reply: " << std::endl;
		for (size_t i = 0; i < m_LuxBeamReply.size(); i++)
		{
			std::cout << (int)m_LuxBeamReply[i] << " ";
		}
		std::cout << std::endl;
	}
}