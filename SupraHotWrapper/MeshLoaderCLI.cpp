#pragma once

#include "MeshLoaderCLI.h"
#include "Vec3MaterialProperty.h"
#include <msclr\marshal_cppstd.h>
#include <Entity.h>
#include <EntityManager.h>
#include <StringUtil.h>
#include "MeshComponentCLI.h"

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

		SupraHot::CLI::EntityCLI^ MeshLoaderCLI::LoadSFHM(System::String^ pathToFile)
		{
			std::string path = msclr::interop::marshal_as<std::string>(pathToFile);
			
			std::vector<SupraHot::MeshComponent*> meshComponents = Instance->Load(path);

			std::unordered_map<std::string, uint32> map;

			Entity* parent = new Entity();

			if (meshComponents.size() > 1)
			{
				parent->SetName(SupraHot::Utils::StringUtil::GetFileNameFromPath(path));

				for (SupraHot::MeshComponent* meshComponent : meshComponents)
				{
					Entity* entity = new Entity();
					parent->AddChild(entity);
					entity->AddComponent(meshComponent);
					
					if (map.find(meshComponent->GetMeshData()->Name) != map.end())
					{
						entity->SetName(meshComponent->GetMeshData()->Name + "(" + std::to_string(map[meshComponent->GetMeshData()->Name]) + ")");
					}
					else
					{
						entity->SetName(meshComponent->GetMeshData()->Name);
					}

					map[meshComponent->GetMeshData()->Name] += 1;
				}
			}
			else if (meshComponents.size() > 0)
			{
				parent->AddComponent(meshComponents[0]);
				parent->SetName(meshComponents[0]->GetMeshData()->Name);
			}
			
			SupraHot::CLI::EntityCLI^ entityCLI = gcnew SupraHot::CLI::EntityCLI();
			entityCLI->ReplaceInstance(parent);

			EntityManager::GetInstance()->AddEntity(parent);
			return entityCLI;
		}
	};
};