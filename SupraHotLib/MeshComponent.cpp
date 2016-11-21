#include "MeshComponent.h"
#include "MeshData.h"
#include "MeshDataRenderer.h"

namespace SupraHot
{
	MeshComponent::MeshComponent(Graphics::MeshData* meshData)
	{
		MeshData = meshData;
	}

	MeshComponent::~MeshComponent()
	{
	}

	void MeshComponent::Registered()
	{
		printf("Registered MeshComponent\nAdded to MeshDataRenderer\n");
		MeshDataRenderer::GetInstance().AddMeshComponent(this);
	}

	void MeshComponent::Removed()
	{
		printf("Removed MeshComponent\nRemoved from MeshDataRenderer\n");
		MeshDataRenderer::GetInstance().RemoveMeshComponent(this);
	}

	void MeshComponent::Destroy()
	{
		printf("Triggered destroy()-method on MeshComponent\n");
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
		return &MeshData->Material;
	}

	Graphics::MeshData* MeshComponent::GetMeshData()
	{
		return MeshData;
	}

	Graphics::Shader* MeshComponent::GetShader()
	{
		return Shader;
	}
};
