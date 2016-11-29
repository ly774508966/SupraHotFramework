#include "MeshComponentCLIImpl.h"

namespace SupraHot
{
	namespace CLI
	{
		MeshComponentCLIImpl::MeshComponentCLIImpl() : MeshComponent(nullptr, nullptr)
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
