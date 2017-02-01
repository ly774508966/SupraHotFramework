#include "MaterialCacheCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		MaterialCacheCLI::MaterialCacheCLI()
		{
			Instance = SupraHot::Graphics::MaterialCache::GetInstance();
		}

		MaterialCacheCLI^ MaterialCacheCLI::GetIntance()
		{
			if (SingletonInstance == nullptr)
			{
				SingletonInstance = gcnew MaterialCacheCLI();
			}

			return SingletonInstance;
		}

		void MaterialCacheCLI::LoadIntoCache(System::String^ pathToMaterialFile)
		{
			std::string pathToMaterialFileStd = msclr::interop::marshal_as<std::string>(pathToMaterialFile);
			Instance->LoadIntoCache(pathToMaterialFileStd);
		}

		Material^ MaterialCacheCLI::GetCached(System::String^ pathToMaterialFile)
		{
			std::string pathToMaterialFileStd = msclr::interop::marshal_as<std::string>(pathToMaterialFile);
			SupraHot::Graphics::MaterialInputsPtr materialPtr = Instance->GetCached(pathToMaterialFileStd);
			
			auto material = gcnew Material(materialPtr.get());
			material->IsCopy = true;

			return material;
		}

		bool MaterialCacheCLI::IsCached(System::String^ pathToMaterialFile)
		{
			std::string pathToMaterialFileStd = msclr::interop::marshal_as<std::string>(pathToMaterialFile);
			return Instance->IsCached(pathToMaterialFileStd);
		}

		MaterialCacheCLI::~MaterialCacheCLI()
		{
			
		}
	};
};
