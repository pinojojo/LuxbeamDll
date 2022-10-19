#pragma once
#include <fstream>
#include <vector>
#include <iostream>

class Sequencer
{
public:
	void LoadFromFile(std::string filePath);



	std::vector<unsigned char> m_Record;
private:
	void GenerateRecord();
	void AppendToRecord(std::vector<unsigned char> bytes);


	std::string m_Contents;
	
	


};

