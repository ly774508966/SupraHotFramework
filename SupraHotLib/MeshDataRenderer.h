#pragma once
#include <vector>
#include "Frustum.h"
#include "RenderCommandQueue.h"
#include "Texture2D.h"

namespace SupraHot
{
	namespace Math
	{
		class Transform;
	}

	namespace Graphics
	{
		class FrameBufferObject;
		class Camera;
		class Shader;
		class MeshData;
		class Camera;
		class GBuffer;
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

		// Graphics pipeline
		Graphics::GBuffer* GBuffer;
		std::vector<Graphics::Texture2DPtr> RenderTargetQueue;
		uint32 RenderTargetQueueSize = 4;

		void RebuildRenderCommandQueue();
		void ClearRenderCommandQueue();
	public:
		static MeshDataRenderer& GetInstance();
		void Initialize(Graphics::Camera* camera, uint32 width, uint32 height);
		void Resize(uint32 width, uint32 height);

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