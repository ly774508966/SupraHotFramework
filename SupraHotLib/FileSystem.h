#pragma once
#include <stdio.h>
#include <string>

#ifdef PLATFORM_ANDROID
#include <android/asset_manager.h>
#endif

namespace SupraHot
{
	namespace Utils
	{
		class FileSystem
		{
		private:
			FileSystem();

			std::string RootPath;
			std::string GetFilePath(std::string directory, std::string fileName);
#ifdef PLATFORM_ANDROID
			AAssetManager* AssetManager;
#endif
		public:
			static FileSystem* GetInstance();
			void SetRootPath(std::string rootPath);
			bool FileExists(std::string directory, std::string fileName);
			FILE* GetFile(std::string directory, std::string fileName, std::string mode);
			~FileSystem();

#ifdef PLATFORM_ANDROID
			void SetAssetManager(AAssetManager* assetManager);
#endif
		};

	};
};
