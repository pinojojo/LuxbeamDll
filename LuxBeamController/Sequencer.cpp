#include "Sequencer.h"
#include "LuxHelper.h"

void Sequencer::LoadFromFile(std::string filePath)
{
	std::ifstream seqFile;

	seqFile.open(filePath);
	std::string contents((std::istreambuf_iterator<char>(seqFile)),
		std::istreambuf_iterator<char>());

	m_Contents.clear();
	m_Contents = contents;

	GenerateRecord();
}

void Sequencer::GenerateRecord()
{
	if (m_Contents.size()<8962)
	{
		// Create record
		int TotalSize = 10 + m_Contents.size();
		int RecId = 105;
		int PkgNo = 1;
		int TotalPkg = 1;
		int ContentSize = m_Contents.size();

		m_Record.clear();

		AppendToRecord(lux::ConvertIntToDualBytes(TotalSize));
		AppendToRecord(lux::ConvertIntToDualBytes(RecId));
		AppendToRecord(lux::ConvertIntToDualBytes(PkgNo));
		AppendToRecord(lux::ConvertIntToDualBytes(TotalPkg));
		AppendToRecord(lux::ConvertIntToDualBytes(ContentSize));
		
		std::vector<unsigned char> contentToAppend(m_Contents.begin(),m_Contents.end());
		
		AppendToRecord(contentToAppend);
	}
	else
	{
		std::cout << "Error: Sequence File Too Large! " << std::endl;
	}
}

void Sequencer::AppendToRecord(std::vector<unsigned char> bytes)
{
	m_Record.insert(m_Record.end(), bytes.begin(), bytes.end());
}

