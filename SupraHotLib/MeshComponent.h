#pragma once
#include "Component.h"
#include "Platform.h"
#include "Material.h"
#include "MeshData.h"

namespace SupraHot
{
	namespace Graphics 
	{
		class MaterialInputs;
	}

	class MeshComponent : public Component
	{	
	protected:
		Graphics::MeshDataPtr MeshData;
		Graphics::Material* Material = nullptr;

		std::string ModelFilePath = "";
		uint32 ModelFileArrayIndex = 0;

	public:
		std::string Identifier = "MeshComponent";
	
		MeshComponent(Graphics::MeshDataPtr meshData, Graphics::Material* material = nullptr, std::string modelFilePath = "", uint32 modelFileArrayIndex = 0);
		~MeshComponent();

		void Registered() override;
		void Removed() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;

		Graphics::Material* GetMaterial();
		Graphics::MeshDataPtr GetMeshData();

		void SetMaterial(Graphics::Material* material);
		
		std::string GetModelFilePath();
		uint32 GetModelFileArrayIndex();

		void UpdateShaderPermution();
	};
};