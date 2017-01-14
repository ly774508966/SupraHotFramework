#pragma once
#include "Component.h"
#include "Platform.h"
#include "ShaderMaterial.h"

namespace SupraHot
{
	namespace Graphics 
	{
		class ShaderMaterial;
		class MeshData;
	}

	class MeshComponent : public Component
	{	
	protected:
		Graphics::MeshData* MeshData = nullptr;
		Graphics::ShaderMaterial* Material = nullptr;

		std::string ModelFilePath = "";
		uint32 ModelFileArrayIndex = 0;

	public:
		std::string Identifier = "MeshComponent";
	
		MeshComponent(Graphics::MeshData* meshData, Graphics::ShaderMaterial* material = nullptr, std::string modelFilePath = "", uint32 modelFileArrayIndex = 0);
		~MeshComponent();

		void Registered() override;
		void Removed() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;

		Graphics::ShaderMaterial* GetMaterial();
		Graphics::MeshData* GetMeshData();
		
		std::string GetModelFilePath();
		uint32 GetModelFileArrayIndex();

		void UpdateShaderPermution();
		void ChangeShader(Graphics::ShaderDescription* shaderDescription);
	};
};