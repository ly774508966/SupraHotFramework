#pragma once
#include <MeshComponent.h>
#include <MeshData.h>
#include <Material.h>

namespace SupraHot
{
	namespace CLI
	{
		class MeshComponentCLIImpl : public SupraHot::MeshComponent
		{
		public:
			MeshComponentCLIImpl();
			MeshComponentCLIImpl(Graphics::MeshData* meshData, Graphics::Material* material);
			void SetMaterial(Graphics::Material* material);
			void SetMeshData(Graphics::MeshData* meshData);
		};
	};
};
