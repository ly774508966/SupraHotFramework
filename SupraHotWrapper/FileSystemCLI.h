#pragma once
#include <Platform.h>
#include <FileSystem.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class FileSystemCLI 
		{
		private:
			SupraHot::Utils::FileSystem* Instance;
			static FileSystemCLI^ SingletonInstance;
			FileSystemCLI();
		public:
			
			~FileSystemCLI();

			static FileSystemCLI^ GetIntance();
			System::String^ GetRootPath();
			bool FileExists(System::String^ pathToFile);
		};
	};
};