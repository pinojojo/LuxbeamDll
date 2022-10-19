#include "LuxImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"
#include <stdlib.h>
void LuxImage::GenerateTemplate()
{
	Pattern FirstPattern;

	for (size_t row = 0; row < DMD_HEIGHT; row++)
	{
		for (size_t col = 0; col < DMD_WIDTH; col++)
		{
			if (int(row / 25) % 2)
			{
				FirstPattern.Assign(row, col, true);
			}
			else
			{
				FirstPattern.Assign(row, col, false);
			}
		}
	}

	Add(FirstPattern);
	//Add(SecondPattern);
	//Add(FirstPattern);
}

void LuxImage::LoadFromBmpFile(const char* filePath)
{
	int width, height, channels;
	unsigned char* dataLoaded = stbi_load(filePath, &width, &height, &channels, 0);

	Clear();

	if (channels)
	{
		if ((width == 1920) && (height == 1080))
		{
			Pattern pat;

			for (size_t row = 0; row < height; row++)
			{
				for (size_t col = 0; col < width; col++)
				{
					int srcVal = (int)dataLoaded[col * channels + row * width * channels];

					if (srcVal > 128)
					{
						pat.Assign(row, col, true);
					}
					else
					{
						pat.Assign(row, col, false);
					}
				}
			}

			Add(pat);
		}
	}
	else
	{
	}
}

void LuxImage::LoadFromBytes(unsigned char* data, int size)
{
	Clear();

	int internal_size = DMD_WIDTH * DMD_HEIGHT;

	if (internal_size == size)
	{
		lux::LogToFile("copying");

		// Pattern is always 1920*1080
		Pattern pat;

		for (size_t row = 0; row < DMD_HEIGHT; row++)
		{
			for (size_t col = 0; col < DMD_WIDTH; col++)
			{
				int srcVal = int(data[col + row * DMD_WIDTH]);

				if (srcVal > 128)
				{
					pat.Assign(row, col, true);
				}
				else
				{
					pat.Assign(row, col, false);
				}
			}
		}

		// Add this pattern to image data.
		Add(pat);
	}
}

void LuxImage::LoadFromCompactBytes(unsigned char* data, int bytes)
{
	Clear();

	int total_rows = bytes * 8 / DMD_WIDTH;

	if (total_rows > 65535)
	{
		lux::LogToFile("image rows larger than 65535");
		return;
	}

	if (m_Rows != total_rows)
	{
		Resize(total_rows);
	}

	// copy data to image data
	memcpy(m_ImageData.data(), data, bytes);
}

void LuxImage::Resize(int rows)
{
	m_Rows = rows;

	m_ImageData.clear();

	int byte_num = DMD_WIDTH / 8 * rows;

	m_ImageData.resize(byte_num);
}

void LuxImage::Add(Pattern& pattern)
{
	m_ImageData.insert(m_ImageData.end(), pattern.data.begin(), pattern.data.end());
	m_Rows += DMD_HEIGHT;
}

void LuxImage::Clear()
{
	m_ImageData.clear();
	m_Packets.clear();
	m_Rows = 0;
}

void LuxImage::MakePackets(int inum)
{
	int PacketNum = m_ImageData.size() / PAYLOADSIZE;
	int PacketSize = PAYLOADSIZE + 14;
	int RecordId = 104;
	int Inum = inum;
	int LineOffset;

	for (size_t PacketId = 0; PacketId < PacketNum; PacketId++)
	{
		LineOffset = 36 * PacketId;

		// Create a packet
		ImagePacket pack;

		// First Part: 14 bytes
		pack.Append(lux::ConvertIntToDualBytes(PacketSize));
		pack.Append(lux::ConvertIntToDualBytes(RecordId));
		pack.Append(lux::ConvertIntToDualBytes(1 + PacketId));// seq_no
		pack.Append(lux::ConvertIntToQuadBytes(Inum));
		pack.Append(lux::ConvertIntToQuadBytes(LineOffset));

		// Second Part: 8640 Bytes
		pack.Append(m_ImageData, PacketId * PAYLOADSIZE);// payload

		//pack.Print();

		m_Packets.push_back(pack);
	}
}

int LuxImage::GetWidth()
{
	return m_Cols;
}

int LuxImage::GetHeight()
{
	return m_Rows;
}

void LuxImage::SetTemplate(int width, int cycle)
{
	Clear();

	Pattern pat;

	for (size_t row = 0; row < DMD_HEIGHT; row++)
	{
		for (size_t col = 0; col < DMD_WIDTH; col++)
		{
			int rowIdLocal = row % cycle;
			if (rowIdLocal < width)
			{
				pat.Assign(row, col, true);
			}
			else
			{
				pat.Assign(row, col, false);
			}
		}
	}

	Add(pat);
}

void LuxImage::SetTemplate(int width, int cycle, int fovWidth, int fovHeight)
{
	Clear();
	Pattern pat;

	// 条纹
	if (m_Type == 0)
	{
		for (size_t row = 0; row < DMD_HEIGHT; row++)
		{
			for (size_t col = 0; col < DMD_WIDTH; col++)
			{
				int rowIdLocal = row % cycle;

				bool isInFov = true;

				isInFov &= (abs(int(row - DMD_HEIGHT / 2)) < (fovHeight / 2));
				isInFov &= (abs(int(col - DMD_WIDTH / 2)) < (fovWidth / 2));

				if ((rowIdLocal < width) && (isInFov))
				{
					pat.Assign(row, col, true);
				}
				else
				{
					pat.Assign(row, col, false);
				}
			}
		}
	}

	// 棋盘格
	if (m_Type == 1)
	{
		for (size_t row = 0; row < DMD_HEIGHT; row++)
		{
			for (size_t col = 0; col < DMD_WIDTH; col++)
			{
				int grid_id_hor = col / width;
				int grid_id_ver = row / width;

				int grid_id = grid_id_hor + grid_id_ver % 2;

				bool isInFov = true;
				isInFov &= (abs(int(row - DMD_HEIGHT / 2)) < (fovHeight / 2));
				isInFov &= (abs(int(col - DMD_WIDTH / 2)) < (fovWidth / 2));

				if ((grid_id % 2) && (isInFov))
				{
					pat.Assign(row, col, true);
				}
				else
				{
					pat.Assign(row, col, false);
				}
			}
		}
	}

	// 竖条纹
	if (m_Type == 2)
	{
		for (size_t row = 0; row < DMD_HEIGHT; row++)
		{
			for (size_t col = 0; col < DMD_WIDTH; col++)
			{
				int rowIdLocal = col % cycle;

				bool isInFov = true;

				isInFov &= (abs(int(row - DMD_HEIGHT / 2)) < (fovHeight / 2));
				isInFov &= (abs(int(col - DMD_WIDTH / 2)) < (fovWidth / 2));

				if ((rowIdLocal < width) && (isInFov))
				{
					pat.Assign(row, col, true);
				}
				else
				{
					pat.Assign(row, col, false);
				}
			}
		}
	}

	Add(pat);
}

void LuxImage::SetTemplateType(int type)
{
	m_Type = type;
}

void LuxImage::MakeZeroImage(bool zero)
{
	Clear();

	Pattern pat;

	for (size_t row = 0; row < DMD_HEIGHT; row++)
	{
		for (size_t col = 0; col < DMD_WIDTH; col++)
		{
			pat.Assign(row, col, zero);
		}
	}

	Add(pat);
}

unsigned char* LuxImage::GetRGBViewable()
{
	m_Viewable.clear();
	for (size_t i = 0; i < m_ImageData.size(); i++)
	{
		auto eightBinaryPixels = lux::ConvertByteToEightPixels(m_ImageData[i]);
		std::vector<unsigned char> eightColorfulPixels(24);
		for (size_t pixelId = 0; pixelId < 8; pixelId++)
		{
			eightColorfulPixels[pixelId * 3 + 0] = eightBinaryPixels[pixelId] ? 255 : 0;
			eightColorfulPixels[pixelId * 3 + 1] = eightBinaryPixels[pixelId] ? 255 : 0;
			eightColorfulPixels[pixelId * 3 + 2] = eightBinaryPixels[pixelId] ? 255 : 0;
		}
	}

	return m_Viewable.data();
}

void Pattern::Assign(int row, int col, bool val)
{
	int pixel_id = row * DMD_WIDTH + col;
	int mem_id = pixel_id / 8;
	int bit_id = 7 - pixel_id % 8; // 左边是高位，右边是低位

	uint8_t mem = data[mem_id];
	mem &= (~(1 << bit_id)); // zero the bit
	mem |= (val << bit_id);  // copy the bit
	data[mem_id] = mem;
}