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