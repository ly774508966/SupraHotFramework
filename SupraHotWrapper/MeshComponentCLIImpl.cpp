#include "MeshComponentCLIImpl.h"

namespace SupraHot
{
	namespace CLI
	{
		MeshComponentCLIImpl::MeshComponentCLIImpl() : MeshComponent(nullptr, nullptr)
		{
		}

		MeshComponentCLIImpl::MeshComponentCLIImpl(Graphics::MeshData* meshData, Graphics::Material* material) : MeshComponent(meshData, material)
		{
		}

		void MeshComponentCLIImpl::SetMaterial(Graphics::Material* material)
		{
			Material = material;
		}

		void MeshComponentCLIImpl::SetMeshData(Graphics::MeshData* meshData)
		{
			MeshData = meshData;
		}
	};
};
