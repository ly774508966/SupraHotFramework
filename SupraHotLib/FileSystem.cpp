#include "FileSystem.h"
#include <fstream>
#include <stdio.h>
#include "Platform.h"

namespace SupraHot
{
	namespace Utils
	{
		FileSystem::FileSystem()
		{
		}


		FileSystem::~FileSystem()
		{
		}

		FileSystem* FileSystem::GetInstance()
		{
			static FileSystem* instance(new FileSystem);
			return instance;
		}

		void FileSystem::SetRootPath(std::string rootPath)
		{
			this->RootPath = rootPath;
		}

		bool FileSystem::FileExists(std::string directory, std::string fileName)
		{
#ifdef PLATFORM_WINDOWS
			std::ifstream infile(GetFilePath(directory, fileName).c_str());
			return infile.good();
#endif

#ifdef PLATFORM_ANDROID
			return false;
#endif
		}


		FILE* FileSystem::GetFile(std::string directory, std::string fileName, std::string mode)
		{
#ifdef PLATFORM_WINDOWS
			FILE* file = nullptr;
			fopen_s(&file, GetFilePath(directory, fileName).c_str(), mode.c_str());
			return file;
#endif

#ifdef PLATFORM_ANDROID
			FILE* file = nullptr;
			return file;
#endif
		}


#ifdef PLATFORM_ANDROID
		void FileSystem::SetAssetManager(AAssetManager* assetManager)
		{
			this->AssetManager = assetManager;
		}
#endif

		std::string FileSystem::GetFilePath(std::string directory, std::string fileName)
		{
			return directory.length() > 0 ? RootPath + directory + "/" + fileName : RootPath + fileName;
		}
	};
};