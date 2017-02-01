#include "MeshComponentCLI.h"
#include <GenericSerializer.h>
#include <MaterialCache.h>

namespace SupraHot
{
	namespace CLI
	{
		MeshComponentCLI::MeshComponentCLI()
		{
			Instance = new SupraHot::MeshComponent(nullptr, nullptr);
		}

		MeshComponentCLI::~MeshComponentCLI()
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("~MeshComponentCLI \n");
#endif
		}

	/*	MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::MeshData^ meshData, SupraHot::CLI::Material^ material)
		{
			Instance = new SupraHot::MeshComponent(meshData->GetHandle(), material->GetHandle());

			Vec3 origin = meshData->GetHandle()->Origin;
			Origin = gcnew Vec3CLI(origin.x, origin.y, origin.z);
		}*/

		MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::ComponentCLI^ componentCLI)
		{
			Instance = componentCLI->GetHandle();
		}

		SupraHot::CLI::Material^ MeshComponentCLI::GetMaterial()
		{
			if (Material == nullptr)
			{
				SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);

				// WIRED UP HERE
				SupraHot::Graphics::MaterialInputs* material = meshComponent->GetMaterial()->GetMaterialInputs().get();

				Material = gcnew SupraHot::CLI::Material();
				Material->IsCopy = true;
				Material->ReplaceInstance(material);
				Material->CheckExistingMaterialProperties();
			}

			return Material;
		}

		void MeshComponentCLI::UpdateShaderPermuation()
		{
			SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
			meshComponent->UpdateShaderPermution();
		}

		System::String^ MeshComponentCLI::GetMeshName()
		{
			return msclr::interop::marshal_as<System::String^>(static_cast<SupraHot::MeshComponent*>(Instance)->GetMeshData()->Name);
		}

		unsigned int MeshComponentCLI::GetModelFileArrayIndex()
		{
			return static_cast<SupraHot::MeshComponent*>(Instance)->GetModelFileArrayIndex();
		}

		System::String^ MeshComponentCLI::GetModelFilePath()
		{
			return msclr::interop::marshal_as<System::String^>(static_cast<SupraHot::MeshComponent*>(Instance)->GetModelFilePath());
		}

		void MeshComponentCLI::SetMaterial(System::String^ materialFilePath)
		{
			SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
			std::string materialFilePathStd = msclr::interop::marshal_as<std::string>(materialFilePath);

			if (SupraHot::Graphics::MaterialCache::GetInstance()->IsCached(materialFilePathStd))
			{
				auto materialInputsPtr = SupraHot::Graphics::MaterialCache::GetInstance()->GetCached(materialFilePathStd);
				SupraHot::Graphics::Material* materialComposition = new SupraHot::Graphics::Material(materialInputsPtr);
				meshComponent->SetMaterial(materialComposition);

				Material = gcnew SupraHot::CLI::Material(materialInputsPtr.get());
				Material->CheckExistingMaterialProperties();
				Material->IsCopy = true;
			}
			else
			{
				SupraHot::Graphics::MaterialCache::GetInstance()->LoadIntoCache(materialFilePathStd);
				auto materialInputsPtr = SupraHot::Graphics::MaterialCache::GetInstance()->GetCached(materialFilePathStd);

				SupraHot::Graphics::Material* materialComposition = new SupraHot::Graphics::Material(materialInputsPtr);
				meshComponent->SetMaterial(materialComposition);

				Material = gcnew SupraHot::CLI::Material(materialInputsPtr.get());
				Material->CheckExistingMaterialProperties();
				Material->IsCopy = true;
			}
		}

		Vec3CLI^ MeshComponentCLI::GetMeshDataOrigin()
		{
			return Origin;
		}
	};
};