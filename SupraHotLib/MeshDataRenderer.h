#pragma once
#include <vector>
#include "Mat4.h"
#include "Frustum.h"
#include <unordered_map>
#include "RenderCommand.h"
#include "RenderCommandQueue.h"

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
		class Camera;
	}

	class MeshComponent;

	class MeshDataRenderer
	{
	private:
		MeshDataRenderer();
		std::vector<MeshComponent*> MeshComponents;
		std::vector<MeshComponent*> TransparentComponents;
		Graphics::RenderCommandQueue RenderCommandQueue;

		// Frustum culling
		Graphics::Frustum CameraFrustum;
		Graphics::Camera* Camera;

		void RebuildRenderCommandQueue();
		void ClearRenderCommandQueue();
	public:
		static MeshDataRenderer& GetInstance();
		void Initialize(Graphics::Camera* camera);

		void AddMeshComponent(MeshComponent* meshComponent);
		void RemoveMeshComponent(MeshComponent* meshComponent);
		void Render(Graphics::Camera* camera);

		// Temp funcs
		void RenderMain(Graphics::Camera* camera);
		void RenderTransparency(std::vector<MeshComponent*>& transparentMeshcomponents, Graphics::Camera* camera);
		void RenderOpaque(std::vector<MeshComponent*>& opaqueMeshComponents, Graphics::Camera* camera);

		void ExecuteRenderCommandQueue();
		
		std::vector<MeshComponent*>& GetMeshComponents();

		// Temp function
		~MeshDataRenderer();
	};
};