#include "MeshComponent.h"
#include "MeshData.h"
#include "MeshDataRenderer.h"
#include "GenericSerializer.h"
#include "StringUtil.h"
#include "MeshDataCache.h"

namespace SupraHot
{
	MeshComponent::MeshComponent(Graphics::MeshDataPtr meshData, Graphics::ShaderMaterial* material,
								 std::string modelFilePath, uint32 modelFileArrayIndex)
	{
		MeshData = meshData;
		Material = material;
		ModelFilePath = modelFilePath;
		ModelFileArrayIndex = modelFileArrayIndex;

		Component::Identifier = Identifier;
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
			Material->Destroy();
			delete Material;
			Material = nullptr;
		}
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

	Graphics::ShaderMaterial* MeshComponent::GetMaterial()
	{
		return Material;
	}

	Graphics::MeshDataPtr MeshComponent::GetMeshData()
	{
		return MeshData;
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
		Graphics::Shader* shader = Graphics::ShaderLibrary::GetInstance()->SelectShaderForShaderMaterialAndMeshData(GetMeshData().get(), GetMaterial());
		if (shader != nullptr)
		{
			GetMaterial()->SetShaderPermutation(shader);
		}
	}

	void MeshComponent::ChangeShader(Graphics::ShaderDescription* shaderDescription)
	{
		SHF_PRINTF("MeshComponent::ChangeShader\ntodo: trim white space\n");
		Utils::GenericSerializer serializer("Materials/" + GetMaterial()->Name + ".json");
		serializer.Serialize(GetMaterial());

		Material->RemoveAllMaterialProperties();
		Material->SetShaderDescription(shaderDescription);
		UpdateShaderPermution();
	}
};
