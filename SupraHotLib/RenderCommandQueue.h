#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	class MeshComponent;

	namespace Graphics
	{
		class RenderCommand;
		class Shader;
		class Material;
		class Camera;
		class Frustum;

		struct RenderCommandQueueState
		{
			Graphics::Shader*   ActiveShader = nullptr;
			MeshComponent*	    ActiveMeshComponent = nullptr;
			Graphics::Material* ActiveMaterial = nullptr;
			Graphics::Camera*   ActiveCamera = nullptr;
			Graphics::Frustum*	ActiveFrustum = nullptr;
		};

		class RenderCommandQueue
		{
		private:
			std::vector<RenderCommand*> RenderCommands;

			RenderCommandQueueState State;
		public:			
			RenderCommandQueue();
			~RenderCommandQueue();
			// static RenderCommandQueue& GetInstance();

			void AddCommand(RenderCommand* renderCommand);
			void Execute();
			void Clear();
			size_t Size();
		};
	};
};