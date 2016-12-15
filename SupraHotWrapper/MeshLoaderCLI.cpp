#pragma once

#include "MeshLoaderCLI.h"
#include "Vec3MaterialProperty.h"
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

		System::Collections::Generic::List<SupraHot::CLI::EntityCLI^>^ MeshLoaderCLI::LoadSFHM(System::String^ pathToFile)
		{
			std::string path = msclr::interop::marshal_as<std::string>(pathToFile);
			
			std::vector<SupraHot::MeshComponent*> meshComponents = Instance->Load(path);

			System::Collections::Generic::List<SupraHot::CLI::EntityCLI^>^ cliEntities = gcnew System::Collections::Generic::List<SupraHot::CLI::EntityCLI^>();

			for (SupraHot::MeshComponent* meshComponent : meshComponents)
			{
				Entity* entity = new Entity();
				entity->AddComponent(meshComponent);
				entity->SetName(meshComponent->GetMeshData()->Name);

				SupraHot::CLI::EntityCLI^ entityCLI = gcnew SupraHot::CLI::EntityCLI();
				entityCLI->ReplaceInstance(entity);
				cliEntities->Add(entityCLI);
				//this->Entities.push_back(entity);
			}

			return cliEntities;
		}
	};
};