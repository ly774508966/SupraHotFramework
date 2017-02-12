#pragma once
#include <vector>
#include "Mat4.h"
#include "Frustum.h"
#include <unordered_map>
#include "RenderCommand.h"

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
		std::vector<Graphics::RenderCommand*> RenderCommandQueue;

		// Frustum culling
		Graphics::Frustum CameraFrustum;
	public:
		static MeshDataRenderer& GetInstance();

		void AddMeshComponent(MeshComponent* meshComponent);
		void RemoveMeshComponent(MeshComponent* meshComponent);
		void Render(Graphics::Camera* camera);

		void RenderMain(Graphics::Camera* camera);
		void RenderTransparency(std::vector<MeshComponent*>& transparentMeshcomponents, Graphics::Camera* camera);
		void RenderOpaque(std::vector<MeshComponent*>& opaqueMeshComponents, Graphics::Camera* camera);
		
		std::vector<MeshComponent*>& GetMeshComponents();

		// Temp function
		~MeshDataRenderer();
	};
};