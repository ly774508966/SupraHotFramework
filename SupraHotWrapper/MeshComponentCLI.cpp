#include "MeshComponentCLI.h"
#include <MeshData.h>
#include <Material.h>

namespace SupraHot
{
	namespace CLI
	{
		MeshComponent::MeshComponent()
		{
			
		}

		MeshComponent::MeshComponent(MeshData^ meshData, Material^ material)
		{
			Instance = new MeshComponentCLIImpl(meshData->GetHandle(), material->GetHandle());
		}

		void MeshComponent::SetMeshData(MeshData^ meshData)
		{
			Instance->SetMeshData(meshData->GetHandle());
		}

		void MeshComponent::SetMaterial(Material^ material)
		{
			Instance->SetMaterial(material->GetHandle());
		}
	};
};