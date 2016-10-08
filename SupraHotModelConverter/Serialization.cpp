#include "Serialization.h"

namespace SupraHot
{
	Serialization::Serialization(const std::string pathToFile)
	{
		this->PathToFile = pathToFile;

		modes[0] = "rb";
		modes[1] = "wb";

	}

	Serialization::~Serialization()
	{
	}

	void Serialization::OpenFile(MODE mode)
	{
		fopen_s(&CurrentFile, PathToFile.c_str(), modes[mode].c_str());
	}

	void Serialization::Write(uint64 size, const void* data)
	{
		fwrite(data, 1, size, CurrentFile);
	}

	void Serialization::Read(uint64 size, void* data)
	{
		fread(data, 1, size, CurrentFile);
	}

	void Serialization::CloseFile()
	{
		if (CurrentFile != nullptr)
		{
			fclose(CurrentFile);
		}
	}
};
