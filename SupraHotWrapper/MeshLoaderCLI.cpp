#pragma once

#include "MeshLoaderCLI.h"
#include "Vec3MaterialProperty.h"
#include <msclr\marshal_cppstd.h>
#include <Entity.h>
#include <EntityManager.h>
#include <StringUtil.h>
#include <MaterialCache.h>
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
			
			std::vector<SupraHot::Graphics::MeshDataPtr>* cachedMeshes = Instance->Load(path);
			std::vector<SupraHot::MeshComponent*> meshComponents;

			for (uint32 m = 0, l = static_cast<uint32>(cachedMeshes->size()); m < l; ++m)
			{
				SupraHot::Graphics::MeshDataPtr meshData = cachedMeshes->at(m);
				SupraHot::Graphics::Material* material = new SupraHot::Graphics::Material(SupraHot::Graphics::MaterialCache::GetInstance()->GetMeshDefaultMaterial());
				meshComponents.push_back(new MeshComponent(meshData, material, path, m));
			}


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
			entityCLI->IsCopy = true;

			//EntityManager::GetInstance()->AddEntity(parent);
			return entityCLI;
		}
	};
};