#pragma once
#include "Component.h"
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics 
	{
		class Material;
		class MeshData;
		class Shader;
	}

	class MeshComponent : public Component
	{	
	private:
		Graphics::MeshData* MeshData;
		Graphics::Shader* Shader;

	public:
		std::string Identifier = "MeshComponent";
	
		MeshComponent(Graphics::MeshData* meshData);
		~MeshComponent();

		void Registered() override;
		void Removed() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;

		Graphics::Material* GetMaterial();
		Graphics::MeshData* GetMeshData();
		Graphics::Shader* GetShader();
	};
};