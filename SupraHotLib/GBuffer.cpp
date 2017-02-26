#include "GBuffer.h"
#include "Shader.h"
#include "Publisher.h"

namespace SupraHot
{
	namespace Graphics
	{
		GBuffer::GBuffer(uint32 width, uint32 height)
		{
			glGenFramebuffers(1, &FrameBufferHandle);
			glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferHandle);

			Width = width;
			Height = height;

			PositionsRT = std::make_shared<Texture2D>(GL_RGB, GL_RGB32F, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			PositionsRT->SetName("GBuffer | Position");

			NormalsRT = std::make_shared<Texture2D>(GL_RGB, GL_RGB16F, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			NormalsRT->SetName("GBuffer | Normals");

			//DepthRT = std::make_shared<Texture2D>(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT32F, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			DepthRT = std::make_shared<Texture2D>(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT24, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			DepthRT->SetName("GBuffer | Depth");

			AlbedoRT = std::make_shared<Texture2D>(GL_RGB, GL_RGB8, GL_FLOAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
			AlbedoRT->SetName("GBuffer | Albedo");

			Resize(width, height);

			BindDefaultRenderTargets();
			CheckStatus();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		GBuffer::~GBuffer()
		{
		}

		void GBuffer::BindDefaultRenderTargets()
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PositionsRT->GetID(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalsRT->GetID(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, AlbedoRT->GetID(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthRT->GetID(), 0);

			GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_NONE };

			glDrawBuffers(4, drawBuffers);
		}

		void GBuffer::CheckStatus()
		{
			int i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			
			if (i != GL_FRAMEBUFFER_COMPLETE) {
				fprintf(stderr, "Error while creating GBuffer: %d \n", i);
			} else {
				fprintf(stderr, "GBuffer online: %d \n", FrameBufferHandle);
			}
		}

		void GBuffer::Attach()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferHandle);
			BindDefaultRenderTargets();

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void GBuffer::Detach()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void GBuffer::SetPixelSize(Graphics::Shader* shader)
		{
			shader->SetVec2(glGetUniformLocation(shader->GetShaderID(), "pixelSize"), Vec2(static_cast<float>(1.0f / Width), static_cast<float>(1.0f / Height)));
		}

		Texture2DPtr& GBuffer::GetPositionsRT()
		{
			return PositionsRT;
		}

		Texture2DPtr& GBuffer::GetNormalsRT()
		{
			return NormalsRT;
		}

		Texture2DPtr& GBuffer::GetDepthRT()
		{
			return DepthRT;
		}

		Texture2DPtr& GBuffer::GetAlbedoRT()
		{
			return AlbedoRT;
		}

		void GBuffer::Resize(uint32 width, uint32 height)
		{
			Width = width;
			Height = height;

			PositionsRT->Destroy();
			PositionsRT->Init(width, height);

			NormalsRT->Destroy();
			NormalsRT->Init(width, height);

			DepthRT->Destroy();
			DepthRT->Init(width, height);

			AlbedoRT->Destroy();
			AlbedoRT->Init(width, height);

			SHF_PRINTF("GBuffer::Resize(%d, %d) \n", width, height);
		}

	};
};