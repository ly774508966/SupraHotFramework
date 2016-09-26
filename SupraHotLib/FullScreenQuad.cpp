#include "FullScreenQuad.h"
#include "Shader.h"

namespace SupraHot
{
	namespace Graphics
	{
		FullScreenQuad::FullScreenQuad()
		{
		}

		FullScreenQuad::~FullScreenQuad()
		{
		}

		uint32 FullScreenQuad::GetVAOHandle()
		{
			return VAOHandle;
		}

		uint32 FullScreenQuad::GetVertexHandle()
		{
			return VertexHandle;
		}

		uint32 FullScreenQuad::GetUVHandle()
		{
			return UVHandle;
		}

		// Create Quad
		void FullScreenQuad::Init()
		{
#if DEVELOPMENT == 1
			printf("Init FullScreenQuad \n");
#endif

			glGenVertexArrays(1, &VAOHandle);
			glBindVertexArray(VAOHandle);

			glGenBuffers(1, &VertexHandle);
			glGenBuffers(1, &UVHandle);

			float vertices[] = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f };
			float uvCoords[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f };

			glBindBuffer(GL_ARRAY_BUFFER, VertexHandle);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &vertices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, UVHandle);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), &uvCoords[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

#if DEVELOPMENT == 1
			printf("FullScreenQuadGL #%d initialized \n", VAOHandle);
#endif
		}

		// Clear VBO's
		void FullScreenQuad::Destroy()
		{
			if (VAOHandle > 0)
			{
				glDeleteVertexArrays(1, &VAOHandle);
			}

			if (VertexHandle > 0)
			{
				glDeleteBuffers(1, &VertexHandle);
			}

			if (UVHandle > 0)
			{
				glDeleteBuffers(1, &UVHandle);
			}

#if DEVELOPMENT == 1
			printf("FullScreenQuad #%d destroyed \n", VAOHandle);
#endif
		}

		void FullScreenQuad::Draw()
		{
			// 3 vertices � 2 floats
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// TODO: refactor this.
		void FullScreenQuad::Attach(Shader* shader)
		{
			// bind fbo render quad
			glBindVertexArray(GetVAOHandle());

			/* Render Quad Setup */
			int vertexpos = glGetAttribLocation(shader->GetShaderID(), "vertexpos");
			int uvPos = glGetAttribLocation(shader->GetShaderID(), "uvPos");

			glBindBuffer(GL_ARRAY_BUFFER, GetVertexHandle());
			glEnableVertexAttribArray(vertexpos);
			glVertexAttribPointer(vertexpos, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));

			glBindBuffer(GL_ARRAY_BUFFER, GetUVHandle());
			glEnableVertexAttribArray(uvPos);
			glVertexAttribPointer(uvPos, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void*>(0));
		}

		void FullScreenQuad::Detach()
		{
			glBindVertexArray(0);
		}
	};
};