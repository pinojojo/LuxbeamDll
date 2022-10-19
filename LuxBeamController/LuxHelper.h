#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace lux
{
	std::vector<unsigned char> ConvertIntToSingleBytes(int val);

	std::vector<unsigned char> ConvertIntToDualBytes(int val);

	std::vector<unsigned char> ConvertIntToQuadBytes(int val);

	std::vector<bool> ConvertByteToEightPixels(unsigned char val);
	int ConvertDualBytesToInt(std::vector<unsigned char> bytes);
	int ConvertQuadBytesToInt(std::vector<unsigned char> bytes);

	std::string ReplaceBetween(const std::string& s,
		const std::string& start_delim,
		const std::string& stop_delim,
		const std::string& to_replace);

	void LogToFile(std::string info);
}