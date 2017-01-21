#pragma once
#include <vector>

/**
*	Simple mesh data renderer.
*	(Temporary)
*/
namespace SupraHot
{
	namespace Math
	{
		class Transform;
	}

	namespace Graphics
	{
		class Camera;
		class Shader;
		class MeshData;
	}

	class MeshComponent;

	class MeshDataRenderer
	{
	private:
		MeshDataRenderer();
		std::vector<MeshComponent*> MeshComponents;

	public:
		static MeshDataRenderer& GetInstance();

		void AddMeshComponent(MeshComponent* meshComponent);
		void RemoveMeshComponent(MeshComponent* meshComponent);
		void Render(Graphics::Camera* camera);

		// Temp function
		void Render(Graphics::Camera* camera, Graphics::MeshData* meshData, Graphics::Shader* shader);
		~MeshDataRenderer();
	};
};