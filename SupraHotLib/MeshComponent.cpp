#include "MeshComponent.h"
#include "MeshData.h"
#include "MeshDataRenderer.h"

namespace SupraHot
{
	MeshComponent::MeshComponent(Graphics::MeshData* meshData, Graphics::Material* material)
	{
		MeshData = meshData;
		Material = material;
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::Registered()
	{
#if DEVELOPMENT == 1
		SHF_PRINTF("Registered MeshComponent\nAdded to MeshDataRenderer\n");
#endif
		MeshDataRenderer::GetInstance().AddMeshComponent(this);
	}

	void MeshComponent::Removed()
	{
#if DEVELOPMENT == 1
		SHF_PRINTF("Removed MeshComponent\nRemoved from MeshDataRenderer\n");
#endif
		MeshDataRenderer::GetInstance().RemoveMeshComponent(this);
	}

	void MeshComponent::Destroy()
	{
#if DEVELOPMENT == 1
		SHF_PRINTF("Triggered destroy()-method on MeshComponent\n");
		SHF_PRINTF("need to destroy the material and meshdata.\n");
#endif
	}

	void MeshComponent::Update(float deltaTime)
	{
	}

	void MeshComponent::LateUpdate(float deltaTime)
	{
	}

	void MeshComponent::FixedUpdate(float deltaTime)
	{
	}

	Graphics::Material* MeshComponent::GetMaterial()
	{
		return Material;
	}

	Graphics::MeshData* MeshComponent::GetMeshData()
	{
		return MeshData;
	}

	MeshComponent::MeshComponent()
	{
	}

	void MeshComponent::SetMeshData(Graphics::MeshData* meshData)
	{
		MeshData = meshData;
	}

	void MeshComponent::SetMaterial(Graphics::Material* material)
	{
		Material = material;
	}
};
