#pragma once

#include "MeshLoaderCLI.h"
#include <msclr\marshal_cppstd.h>
#include <Entity.h>

namespace SupraHot
{
	namespace CLI
	{
		MeshLoaderCLI::MeshLoaderCLI()
		{
			Instance = SupraHot::Utils::MeshDataLoader::GetInstance();
		}

		MeshLoaderCLI^ MeshLoaderCLI::GetIntance()
		{
			if (SingletonInstance == nullptr)
			{
				SingletonInstance = gcnew MeshLoaderCLI();
			}

			return SingletonInstance;
		}

		void MeshLoaderCLI::LoadSFHM(System::String^ pathToFile)
		{
			std::string path = msclr::interop::marshal_as<std::string>(pathToFile);
			
			std::vector<SupraHot::MeshComponent*> meshComponents = Instance->Load(path);

			for (SupraHot::MeshComponent* meshComponent : meshComponents)
			{
				Entity* entity = new Entity();
				entity->AddComponent(meshComponent);
				//this->Entities.push_back(entity);
			}
		}
	};
};