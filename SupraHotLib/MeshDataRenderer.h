#pragma once
#include <vector>
#include "Mat4.h"
#include "Frustum.h"

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

		// Frustum culling
		Graphics::Frustum CameraFrustum;
		void FrustumCulling();

		void BucketSorting();
	public:
		static MeshDataRenderer& GetInstance();

		void AddMeshComponent(MeshComponent* meshComponent);
		void RemoveMeshComponent(MeshComponent* meshComponent);
		void Render(Graphics::Camera* camera);

		// Temp function
		~MeshDataRenderer();
	};
};