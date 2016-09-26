#pragma once
#include "Platform.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Shader;
		class FullScreenQuad
		{
		private:
			uint32 VAOHandle, VertexHandle, UVHandle;
		public:
			FullScreenQuad();
			~FullScreenQuad();

			uint32 GetVAOHandle();
			uint32 GetVertexHandle();
			uint32 GetUVHandle();

			void Init();
			void Destroy();
			void Draw();

			void Attach(Shader* shader);
			void Detach();
		};
	};
};
