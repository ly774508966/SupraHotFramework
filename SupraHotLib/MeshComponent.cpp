#include "MeshComponent.h"
#include "MeshData.h"
#include "MeshDataRenderer.h"
#include "GenericSerializer.h"
#include "StringUtil.h"
#include "MeshDataCache.h"

namespace SupraHot
{
	MeshComponent::MeshComponent(Graphics::MeshDataPtr meshData, Graphics::Material* material,
								 std::string modelFilePath, uint32 modelFileArrayIndex)
	{
		MeshData = meshData;
		Material = material;
		ModelFilePath = modelFilePath;
		ModelFileArrayIndex = modelFileArrayIndex;

		Component::Identifier = Identifier;

		if (meshData != nullptr && meshData.get() != nullptr && material != nullptr)
		{
			UpdateShaderPermution();
		}
	}

	MeshComponent::~MeshComponent()
	{
#if DEVELOPMENT == 1
		//SHF_PRINTF("~MeshComponent \n");
#endif
	}

	void MeshComponent::Registered()
	{
#if DEVELOPMENT == 1
		//SHF_PRINTF("Registered MeshComponent\nAdded to MeshDataRenderer\n");
#endif
		Parent->GetTransform().SetLocalPosition(MeshData->Origin);
		MeshDataRenderer::GetInstance().AddMeshComponent(this);
	}

	void MeshComponent::Removed()
	{
#if DEVELOPMENT == 1
		//SHF_PRINTF("Removed MeshComponent\nRemoved from MeshDataRenderer\n");
#endif
		MeshDataRenderer::GetInstance().RemoveMeshComponent(this);
	}

	void MeshComponent::Destroy()
	{
#if DEVELOPMENT == 1
		//SHF_PRINTF("Triggered destroy()-method on MeshComponent\n");
		//SHF_PRINTF("need to destroy the material and meshdata.\n");
#endif

		// TODO: Instead of just out right delete the mesh data
		// we should call the MeshDataCache and ask it to free it
		if (MeshData != nullptr)
		{
			Graphics::MeshDataCache::GetInstance()->Free(ModelFilePath, ModelFileArrayIndex, MeshData);
			MeshData = Graphics::MeshDataPtr();

			//MeshData->Destroy();
			//delete MeshData.get();
			//MeshData = nullptr;			
		}
		
		if (Material != nullptr)
		{
			delete Material;
		}
	}

	void MeshComponent::Update(float deltaTime)
	{
		if (GetMaterial()->GetMaterialInputs()->NeedsUpdate())
		{
			UpdateShaderPermution();
		}
	}

	void MeshComponent::LateUpdate(float deltaTime)
	{
		if (GetMaterial()->GetMaterialInputs()->NeedsUpdate())
		{
			GetMaterial()->GetMaterialInputs()->Updated();
		}
	}

	void MeshComponent::FixedUpdate(float deltaTime)
	{
	}

	Graphics::Material* MeshComponent::GetMaterial()
	{
		return Material;
	}

	Graphics::MeshDataPtr MeshComponent::GetMeshData()
	{
		return MeshData;
	}

	void MeshComponent::SetMaterial(Graphics::Material* material)
	{
		if (Material != nullptr)
		{
			delete Material;
		}

		Material = material;
		UpdateShaderPermution();
	}

	std::string MeshComponent::GetModelFilePath()
	{
		return ModelFilePath;
	}

	uint32 MeshComponent::GetModelFileArrayIndex()
	{
		return ModelFileArrayIndex;
	}

	void MeshComponent::UpdateShaderPermution()
	{
		Material->UpdateShaderPermutation(MeshData);
	}
};
