#include "FrameBufferObject.h"
#include "Shader.h"
#include <string>

namespace SupraHot
{
	namespace Graphics
	{
		FrameBufferObject::FrameBufferObject()
		{
		}

		FrameBufferObject::~FrameBufferObject()
		{
		}

		void FrameBufferObject::CheckError()
		{
			printf("Checking FBO status \n");
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			const char *err_str = 0;
			char buf[80];

			if (status != GL_FRAMEBUFFER_COMPLETE)
			{
				switch (status)
				{
				case GL_FRAMEBUFFER_UNSUPPORTED:
					err_str = "UNSUPPORTED";
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					err_str = "INCOMPLETE ATTACHMENT";
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					err_str = "INCOMPLETE MISSING ATTACHMENT";
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					err_str = "INCOMPLETE MULTISAMPLE";
					break;
#ifdef PLATFORM_WINDOWS
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					err_str = "INCOMPLETE DRAW BUFFER";
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
					err_str = "INCOMPLETE READ BUFFER";
					break;
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					err_str = "INCOMPLETE LAYER TARGETS";
					break;
#endif

				default:
					SHF_SPRINTF(buf, "0x%x", status);
					err_str = buf;
					break;
				}

				printf("ERROR: glCheckFramebufferStatus() returned '%s'", err_str);
			}
			else
			{
				printf("SUCCESS: FRAMEBUFFER %u ONLINE \n", FramebufferID);
			}
		}

		void FrameBufferObject::InitRenderTargets()
		{
			if (ColorRenderTarget != nullptr)
			{
				ColorRenderTarget->Destroy();
			}
			else
			{
				ColorRenderTarget = new Texture2D("FBO ColorRenderTarget");
				ColorRenderTarget->SetInternalFormat(GL_RGBA32F);
				ColorRenderTarget->SetFormat(GL_RGBA);
				ColorRenderTarget->SetType(GL_FLOAT);

				ColorRenderTarget->SetWrapS(GL_CLAMP_TO_EDGE);
				ColorRenderTarget->SetWrapT(GL_CLAMP_TO_EDGE);

				ColorRenderTarget->SetMinFilter(GL_LINEAR);
				ColorRenderTarget->SetMagFilter(GL_LINEAR);
			}
		
			// Init the texture
			ColorRenderTarget->Init(Width, Height);

			if (DepthRenderTarget != nullptr)
			{
				DepthRenderTarget->Destroy();
			} 
			else
			{
				DepthRenderTarget = new Texture2D("DepthRenderTarget");

				DepthRenderTarget->SetInternalFormat(GL_DEPTH_COMPONENT32F);
				DepthRenderTarget->SetFormat(GL_DEPTH_COMPONENT);
				DepthRenderTarget->SetType(GL_FLOAT);

				DepthRenderTarget->SetWrapS(GL_CLAMP_TO_EDGE);
				DepthRenderTarget->SetWrapT(GL_CLAMP_TO_EDGE);

				DepthRenderTarget->SetMinFilter(GL_LINEAR);
				DepthRenderTarget->SetMagFilter(GL_LINEAR);
			}
			
			DepthRenderTarget->Init(Width, Height);
		}

		void FrameBufferObject::AttachRenderTargets()
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorRenderTarget->GetID(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthRenderTarget->GetID(), 0);

			GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_NONE };
			glDrawBuffers(2, buffers);
		}

		void FrameBufferObject::Init(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;
			InitRenderTargets();
			
			FBOQuad = new FullScreenQuad();
			FBOQuad->Init();
		}

		void FrameBufferObject::Resize(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;
	
			ColorRenderTarget->Destroy();
			DepthRenderTarget->Destroy();

			ColorRenderTarget->Init(Width, Height);
			DepthRenderTarget->Init(Width, Height);
		}

		void FrameBufferObject::Attach()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FramebufferID);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void FrameBufferObject::Detach()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		void FrameBufferObject::RenderToScreen(Shader* fboToScreenShader)
		{
			/* unbind framebuffer and use final program */
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClear(GL_COLOR_BUFFER_BIT);

			Mat4 projectionMat;
			projectionMat = projectionMat.ProjectOrthographic(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0);

			/* Bind Textures, G-Buffers */
			uint32 shaderProgram = static_cast<Shader*>(fboToScreenShader)->GetShaderID();
			glUseProgram(shaderProgram);

			SetPixelSize(fboToScreenShader);
			fboToScreenShader->SetMat4(glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix"), projectionMat);
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "modelViewProjectionMatrix"), 1, GL_FALSE, &projectionMat.m[0][0]);

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glDisable(GL_BLEND);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, ReadSource->GetID());
			glUniform1i(glGetUniformLocation(shaderProgram, "in_texture"), 0);
			
			FBOQuad->Attach(fboToScreenShader);
			FBOQuad->Draw();
			FBOQuad->Detach();

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void FrameBufferObject::Destroy()
		{
			if (ColorRenderTarget != nullptr)
			{
				ColorRenderTarget->Destroy();
				delete ColorRenderTarget;
			}

			if (DepthRenderTarget != nullptr)
			{
				DepthRenderTarget->Destroy();
				delete DepthRenderTarget;
			}
		}

		void FrameBufferObject::SetPixelSize(Shader* shader)
		{
			uint32 shaderProgram = shader->GetShaderID();
			glUseProgram(shaderProgram);
			shader->SetVec2(glGetUniformLocation(shaderProgram, "pixelsize"), Vec2(1.0f / static_cast<float>(Width), 1.0f / static_cast<float>(Height)));
		}

		FullScreenQuad* FrameBufferObject::GetFullScreenQuad()
		{
			return FBOQuad;
		}

		void FrameBufferObject::SetRenderTarget(Texture2D* renderTarget)
		{
			this->RenderTarget = renderTarget;
		}

		Texture2D* FrameBufferObject::GetColorRenderTarget()
		{
			return ColorRenderTarget;
		}

		Texture2D* FrameBufferObject::GetDepthRenderTarget()
		{
			return DepthRenderTarget;
		}

		void FrameBufferObject::SetReadSource(Texture2D* readSource)
		{
			this->ReadSource = readSource;
		}
	};
};