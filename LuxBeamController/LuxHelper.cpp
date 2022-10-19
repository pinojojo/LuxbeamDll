#include "LuxHelper.h"

namespace lux {
	std::vector<unsigned char> ConvertIntToSingleBytes(int val)
	{
		std::vector<unsigned char> ret(1);

		ret[0] = val & 0xff;          // lo-8

		return ret;
	}

	std::vector<unsigned char> ConvertIntToDualBytes(int val) {
		std::vector<unsigned char> ret(2);

		ret[0] = (val >> 8) & 0xff;	  // hi-8
		ret[1] = val & 0xff;          // lo-8

		return ret;
	}

	std::vector<unsigned char> ConvertIntToQuadBytes(int val)
	{
		{
			std::vector<unsigned char> ret(4);
			ret[0] = (val >> 24) & 0xff;
			ret[1] = (val >> 16) & 0xff;
			ret[2] = (val >> 8) & 0xff;
			ret[3] = val & 0xff;

			return ret;
		}
	}

	int ConvertDualBytesToInt(std::vector<unsigned char> bytes)
	{
		int ret = 0;
		if (bytes.size() == 2)
		{
			ret += bytes[0] << 8;
			ret += bytes[1];
		}

		return ret;
	}

	int ConvertQuadBytesToInt(std::vector<unsigned char> bytes)
	{
		int ret = 0;
		if (bytes.size() == 4)
		{
			ret += bytes[0] << 24;
			ret += bytes[1] << 16;
			ret += bytes[2] << 8;
			ret += bytes[3];
		}

		return ret;
	}

	std::vector<bool> ConvertByteToEightPixels(unsigned char val)
	{
		std::vector<bool> ret(8);
		for (int n = 0; n < 8; n++)
			ret[7 - n] = (val >> n) & 1;

		return ret;
	}

	std::string ReplaceBetween(const std::string& s, const std::string& start_delim, const std::string& stop_delim, const std::string& to_replace)
	{
		size_t first_delim_pos = s.find(start_delim);
		size_t end_pos_of_first_delim = first_delim_pos + start_delim.length();
		size_t last_delim_pos = s.find(stop_delim, end_pos_of_first_delim);

		std::string need_to_replace = s.substr(end_pos_of_first_delim, last_delim_pos - end_pos_of_first_delim);

		std::string ret = s;

		return ret.replace(end_pos_of_first_delim, need_to_replace.length(), to_replace);
	}

	void LogToFile(std::string info)
	{
		std::ofstream fw;
		fw.open("dmd_log.txt", std::ios::app);
		if (fw.is_open())
		{
			//store array contents to text file
			fw << info;
			fw << std::string("\n");
			fw.close();
		}

		fw.close();
	}
}