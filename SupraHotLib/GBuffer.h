#pragma once
#include "Platform.h"
#include "Texture2D.h"
#include "Subscriber.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Shader;

		class GBuffer
		{
		private:
			// General render targets
			Texture2DPtr PositionsRT;
			Texture2DPtr NormalsRT;
			Texture2DPtr DepthRT;
			Texture2DPtr AlbedoRT;			

			uint32 Width = 0, Height = 0;

			uint32 FrameBufferHandle = 0;
			
			void BindDefaultRenderTargets();
			void CheckStatus();
		public:
			GBuffer(uint32 width, uint32 height);
			~GBuffer();

			void Resize(uint32 width, uint32 height);
			void Attach();
			void Detach();
			void SetPixelSize(Graphics::Shader* shader);

			Texture2DPtr& GetPositionsRT();
			Texture2DPtr& GetNormalsRT();
			Texture2DPtr& GetDepthRT();
			Texture2DPtr& GetAlbedoRT();
		};
	};
};