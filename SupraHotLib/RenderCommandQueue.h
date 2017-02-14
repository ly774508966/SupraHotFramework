#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		class RenderCommand;
		class Shader;
		class MeshData;
		class Material;
		class Camera;

		struct RenderCommandQueueState
		{
			Graphics::Shader*   ActiveShader = nullptr;
			Graphics::MeshData* ActiveMeshData = nullptr;
			Graphics::Material* ActiveMaterial = nullptr;
			Graphics::Camera*   ActiveCamera = nullptr;
		};

		class RenderCommandQueue
		{
		private:
			RenderCommandQueue();
			std::vector<RenderCommand*> RenderCommands;

			RenderCommandQueueState State;

			
		public:			
			~RenderCommandQueue();
			static RenderCommandQueue& GetInstance();

			void AddCommand(RenderCommand* renderCommand);
			void Execute();
			void Clear();
		};
	};
};