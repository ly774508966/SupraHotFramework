#include "SkyBox.h"
#include "TextureCube.h"
#include "Texture2D.h"
#include "Camera.h"
#include "Shader.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		SkyBox::SkyBox()
		{
		}

		SkyBox::~SkyBox()
		{
		}

		void SkyBox::SetEnvironmentMap(TextureCube* enviromentMap)
		{
			this->CubeMap = enviromentMap;
		}

		void SkyBox::SetEnvironmentMap(Texture2D* enviromentMap)
		{
			this->SphereMap = enviromentMap;
		}

		TextureCube* SkyBox::GetCubeMap()
		{
			return CubeMap;
		}

		Texture2D* SkyBox::GetSphereMap()
		{
			return SphereMap;
		}

		// TODO: Use Position & Quaternion instead!
		//		This will make this class more resuable. But for now it is ok.
		void SkyBox::Render(Camera* camera, Shader* shader)
		{
			glDisable(GL_DEPTH_TEST);
			// glDepthMask(GL_FALSE);

			// Render Skybox here
			shader->Attach();

			GLuint shaderProgramID = shader->GetShaderID();
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewMatrix"), (*camera->GetViewMatrix()));
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ProjectionMatrix"), (*camera->GetProjectionMatrix()));
			
			if (CubeMap != nullptr)
			{
				shader->SetTextureCube(glGetUniformLocation(shaderProgramID, "CubeMap"), CubeMap, GL_TEXTURE0);
			}
			else if (SphereMap != nullptr)
			{
				shader->SetTexture2D(glGetUniformLocation(shaderProgramID, "SphereMap"), SphereMap, GL_TEXTURE0);
			}
			
			// Draw the vertices
			Render(shader);

			shader->Detach();

			glEnable(GL_DEPTH_TEST);
			// glDepthMask(GL_TRUE);
		}

		void SkyBox::Render(Shader* shader)
		{
			// Bind GL buffers
			glBindVertexArray(VAOHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOHandle);

			glUseProgram(shader->GetShaderID());
			uint32 vertexAttrib = glGetAttribLocation(shader->GetShaderID(), "VertexPosition");
			glBindBuffer(GL_ARRAY_BUFFER, VertexVBOHandle);
			glEnableVertexAttribArray(vertexAttrib);
			glVertexAttribPointer(vertexAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

			// DrawArray.
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);

			glBindVertexArray(0);
		}

		void SkyBox::Init()
		{
			// Create Vertex list
			std::vector<Vec3> vertices;
			std::vector<uint16> indices;
			float scale = 1.0f;

			vertices.push_back(Vec3(-1.0f * scale, 1.0f * scale, 1.0f * scale));
			vertices.push_back(Vec3(-1.0f * scale, -1.0f * scale, 1.0f * scale));
			vertices.push_back(Vec3(1.0f * scale, -1.0f * scale, 1.0f * scale));
			vertices.push_back(Vec3(1.0f * scale, 1.0f * scale, 1.0f * scale));
			vertices.push_back(Vec3(-1.0f * scale, 1.0f * scale, -1.0f * scale));
			vertices.push_back(Vec3(-1.0f * scale, -1.0f * scale, -1.0f * scale));
			vertices.push_back(Vec3(1.0f * scale, -1.0f * scale, -1.0f * scale));
			vertices.push_back(Vec3(1.0f * scale, 1.0f * scale, -1.0f * scale));

			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(0);
			indices.push_back(3);
			indices.push_back(2);

			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(6);
			indices.push_back(3);
			indices.push_back(7);
			indices.push_back(6);

			indices.push_back(4);
			indices.push_back(5);
			indices.push_back(6);
			indices.push_back(4);
			indices.push_back(7);
			indices.push_back(6);

			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(5);
			indices.push_back(0);
			indices.push_back(4);
			indices.push_back(5);

			indices.push_back(0);
			indices.push_back(3);
			indices.push_back(4);
			indices.push_back(3);
			indices.push_back(4);
			indices.push_back(7);

			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(6);
			indices.push_back(1);
			indices.push_back(5);
			indices.push_back(6);

			vertices.shrink_to_fit();
			indices.shrink_to_fit();

			// Create GL buffer
			glGenVertexArrays(1, &VAOHandle);
			glBindVertexArray(VAOHandle);

			glGenBuffers(1, &IndexVBOHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16), &indices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if DEVELOPMENT == 1
			int err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("Error %d happened while generating IndexVBOHandle for skybox \n", err);
			}
#endif
			std::vector<float> verts;
			verts.shrink_to_fit();
			for (uint32 i = 0; i < vertices.size(); i++)
			{
				verts.push_back(vertices.at(i).x);
				verts.push_back(vertices.at(i).y);
				verts.push_back(vertices.at(i).z);
			}

			glGenBuffers(1, &VertexVBOHandle);
			glBindBuffer(GL_ARRAY_BUFFER, VertexVBOHandle);
			glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), &verts[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

#if DEVELOPMENT == 1
			err = glGetError();
			if (err != 0)
			{
				SHF_PRINTF("Error %d happened while generating VertexVBOHandle for skybox \n", err);
			}
#endif
			
			glBindVertexArray(0);
		}

		void SkyBox::Destroy()
		{
			if (CubeMap != nullptr)
			{
				CubeMap->Destroy();
				delete CubeMap;
			}

			if (SphereMap != nullptr)
			{
				SphereMap->Destroy();
				delete SphereMap;
			}
		}

	};
};