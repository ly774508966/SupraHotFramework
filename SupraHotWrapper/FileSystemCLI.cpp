#include "FileSystemCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		FileSystemCLI::FileSystemCLI()
		{
			Instance = SupraHot::Utils::FileSystem::GetInstance();
		}

		FileSystemCLI::~FileSystemCLI()
		{
		}

		FileSystemCLI^ FileSystemCLI::GetIntance()
		{
			if (SingletonInstance == nullptr)
			{
				SingletonInstance = gcnew FileSystemCLI();
			}

			return SingletonInstance;
		}

		System::String^ FileSystemCLI::GetRootPath()
		{
			return msclr::interop::marshal_as<System::String^>(Instance->GetRootPath());
		}

		bool FileSystemCLI::FileExists(System::String^ pathToFile)
		{
			std::string pathToFileStd = msclr::interop::marshal_as<std::string>(pathToFile);
			return Instance->FileExists("", pathToFileStd);
		}
	};
};