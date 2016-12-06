#pragma once
#include "Component.h"
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics 
	{
		class Material;
		class MeshData;
	}

	class MeshComponent : public Component
	{	
	protected:
		Graphics::MeshData* MeshData = nullptr;
		Graphics::Material* Material = nullptr;

	public:
		std::string Identifier = "MeshComponent";
	
		MeshComponent(Graphics::MeshData* meshData, Graphics::Material* material = nullptr);		
		~MeshComponent();

		void Registered() override;
		void Removed() override;
		void Destroy() override;
		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;

		Graphics::Material* GetMaterial();
		Graphics::MeshData* GetMeshData();
	};
};