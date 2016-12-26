#pragma once
#include "Platform.h"
#include "FullScreenQuad.h"
#include "Texture2D.h"
#include "Camera.h"

namespace SupraHot
{
	namespace Graphics
	{
		class FrameBufferObject
		{
		private:
			FullScreenQuad* FBOQuad;

			Texture2D* ColorRenderTarget = nullptr;
			Texture2D* DepthRenderTarget = nullptr;

			uint32 FramebufferID;
			uint32 Width, Height;

			void CheckError();
			void InitRenderTargets();
			void AttachRenderTargets();

			Vec3 ClearColor;

			Texture2D* RenderTarget = nullptr;
			Texture2D* ReadSource = nullptr;

		public:
			FrameBufferObject();
			~FrameBufferObject();
			void Init(uint32 width, uint32 height);
			void Resize(uint32 width, uint32 height);
			void Destroy();
			void Attach();
			void Detach();
			void RenderToScreen(Shader* fboToScreenShader);

			void SetPixelSize(Shader* shader);
			FullScreenQuad* GetFullScreenQuad();

			void SetRenderTarget(Texture2D* renderTarget);
			void SetReadSource(Texture2D* readSource);

			Texture2D* GetColorRenderTarget();
			Texture2D* GetDepthRenderTarget();

			uint32 GetWidth();
			uint32 GetHeight();

			void SetClearColor(float r, float g, float b);
		};
	};
};

