#pragma once
#include "Platform.h"
#include "MeshComponent.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Renderer
		{
		private:
			Renderer();
		public:
			static Renderer& GetInstance();
			~Renderer();

			void RenderSingleMeshComponent(MeshComponent* meshComponent);
		};
	};
};
