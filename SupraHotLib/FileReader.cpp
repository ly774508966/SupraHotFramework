#include "FileReader.h"
#include <iostream>
#include <fstream>

namespace SupraHot
{
	namespace Utils
	{
		FileReader* FileReader::GetInstance()
		{
			static FileReader* instance(new FileReader);
			return instance;
		}

		FileReader::FileReader()
		{
		}

		FileReader::~FileReader()
		{
		}

		std::vector<std::string> FileReader::ReadFile(std::string path)
		{
			std::vector<std::string> fileContent;

			std::ifstream stream;
			stream.open(path);

			if (!stream.is_open())
			{
				printf("Instream could not open file: %s \n", path.c_str());
			}
			else
			{
				char buf[2048];
				while (!stream.eof())
				{
					stream.getline(buf, 2048);
					fileContent.push_back(std::string(buf));
				}
				stream.close();
			}

			return fileContent;
		}
	};
};