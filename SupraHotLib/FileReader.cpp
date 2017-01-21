#include "FileReader.h"
#include "FileSystem.h"

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

			FILE* file = FileSystem::GetInstance()->GetFile("", path, "rb");

			if (file != nullptr)
			{
				char buf[2048];
				while (fgets(buf, 2048, file)) {
					fileContent.push_back(std::string(buf));
				}
				std::fclose(file);
			}
			
			return fileContent;
		}
	};
};