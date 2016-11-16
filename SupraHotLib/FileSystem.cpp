#include "FileSystem.h"
#include <fstream>
#include <stdio.h>
#include "Platform.h"

namespace SupraHot
{
	namespace Utils
	{
		// ---------------------------
		// Android FILE* API Wrapper
		// ---------------------------
		// Source: http://www.50ply.com/blog/2013/01/19/loading-compressed-android-assets-with-file-pointer/

#ifdef PLATFORM_ANDROID
		static int android_read(void* cookie, char* buf, int size) {
			return AAsset_read((AAsset*)cookie, buf, size);
		}

		static int android_write(void* cookie, const char* buf, int size) {
			return EACCES; // can't provide write access to the apk
		}

		static fpos_t android_seek(void* cookie, fpos_t offset, int whence) {
			return AAsset_seek((AAsset*)cookie, offset, whence);
		}

		static int android_close(void* cookie) {
			AAsset_close((AAsset*)cookie);
			return 0;
		}
#endif


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
#ifndef PLATFORM_ANDROID
			std::ifstream infile(GetFilePath(directory, fileName).c_str());
			return infile.good();
#else 
			FILE* f = GetFile(directory, fileName, "rb");
			if (f != nullptr)
			{
				std::fclose(f);
				return true;
			}

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
			AAsset* asset = AAssetManager_open(AssetManager, GetFilePath(directory, fileName).c_str(), 0);
			if (!asset)
			{
				return nullptr;
			}
			return funopen(asset, android_read, android_write, android_seek, android_close);
#endif

#ifdef PLATFORM_EMSCRIPTEN
			FILE* file = fopen(GetFilePath(directory, fileName).c_str(), mode.c_str());
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