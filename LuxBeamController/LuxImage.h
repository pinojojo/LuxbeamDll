#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "LuxHelper.h"

#define  DMD_WIDTH  1920
#define  DMD_HEIGHT 1080
#define  PAYLOADSIZE 8640   //8KB

struct Pattern
{
	Pattern() :data(DMD_WIDTH / 8 * DMD_HEIGHT) {}
	void Assign(int row, int col, bool val);
	std::vector<unsigned char> data;
};

struct ImagePacket
{
	void Clear() { Record.clear(); }
	void Append(std::vector<unsigned char> bytes) {
		Record.insert(std::end(Record), std::begin(bytes), std::end(bytes));
	}

	void Append(std::vector<unsigned char> bytes, int start) {
		auto start_it = bytes.begin() + start;
		auto end_it = bytes.begin() + start + 8640;
		Record.insert(Record.end(), start_it, end_it);
	}

	void Print() {
		std::cout << "Image packet: ";
		for (size_t i = 0; i < Record.size(); i++)
		{
			std::cout << (int)Record[i] << " ";
		}
		std::cout << std::endl;
	}

	int								PacketId;
	int								Inum;
	int								LineOffset = 0;
	std::vector<unsigned char>	    Record;
};

class LuxImage
{
public:
	void GenerateTemplate();

	void LoadFromBmpFile(const char* filePath);
	void LoadFromBytes(unsigned char* data, int size);

	// 加载单bit图像数据
	void LoadFromCompactBytes(unsigned char* data, int bytes);

	void Resize(int rows);
	void Add(Pattern& pattern);
	void Clear();

	void MakePackets(int inum);

	unsigned char* GetRGBViewable();
	int GetWidth();
	int GetHeight();

	// TestImage Setting
	void SetTemplate(int width, int cycle);
	void SetTemplate(int width, int cycle, int fovWidth, int fovHeight);
	void SetTemplateType(int type);
	void MakeZeroImage(bool zero);

public:
	std::vector<ImagePacket>		m_Packets;
	int								m_Rows = 0;
	int								m_Type = 0; //0条纹 1棋盘格
	int								m_Cols = DMD_WIDTH;

private:
	std::vector<unsigned char>		m_ImageData;  // 图像数据
	std::vector<unsigned char>		m_Viewable;
};
