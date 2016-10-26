#include "SkyBox.h"
#include "TextureCube.h"
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
			this->EnviromentMap = enviromentMap;
		}

		TextureCube* SkyBox::GetEnvironmentMap()
		{
			return EnviromentMap;
		}

		// TODO: Use Position & Quaternion instead!
		//		This will make this class more resuable. But for now it is ok.
		void SkyBox::Render(Camera* camera, Shader* shader)
		{
			Vec3 cameraPosition = camera->Position;
			camera->Position.x = camera->Position.y = camera->Position.z = 0;

			camera->ResetMatrices();
			camera->ApplyTranslation(); 
			camera->ApplyRotation();

			camera->CreateViewProjectionMatrix();
			camera->CreateInverseViewProjectionMatrix();

			glDisable(GL_DEPTH_TEST);

			// Render Skybox here
			shader->Attach();

			GLuint shaderProgramID = shader->GetShaderID();
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewProjectionMatrix"), (*camera->GetViewProjectionMatrix()));
			shader->SetTextureCube(glGetUniformLocation(shaderProgramID, "CubeMap"), EnviromentMap, GL_TEXTURE0);
			
			// Draw the vertices
			Render(shader);

			shader->Detach();

			glEnable(GL_DEPTH_TEST);
			camera->Position = cameraPosition;
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
			std::vector<Vec3> Vertices;
			std::vector<uint16> Indices;

			Vertices.push_back(Vec3(-1.0f, 1.0f, 1.0f));
			Vertices.push_back(Vec3(-1.0f, -1.0f, 1.0f));
			Vertices.push_back(Vec3(1.0f, -1.0f, 1.0f));
			Vertices.push_back(Vec3(1.0f, 1.0f, 1.0f));
			Vertices.push_back(Vec3(-1.0f, 1.0f, -1.0f));
			Vertices.push_back(Vec3(-1.0f, -1.0f, -1.0f));
			Vertices.push_back(Vec3(1.0f, -1.0f, -1.0f));
			Vertices.push_back(Vec3(1.0f, 1.0f, -1.0f));

			Indices.push_back(0);
			Indices.push_back(1);
			Indices.push_back(2);
			Indices.push_back(0);
			Indices.push_back(3);
			Indices.push_back(2);

			Indices.push_back(2);
			Indices.push_back(3);
			Indices.push_back(6);
			Indices.push_back(3);
			Indices.push_back(7);
			Indices.push_back(6);

			Indices.push_back(4);
			Indices.push_back(5);
			Indices.push_back(6);
			Indices.push_back(4);
			Indices.push_back(7);
			Indices.push_back(6);

			Indices.push_back(0);
			Indices.push_back(1);
			Indices.push_back(5);
			Indices.push_back(0);
			Indices.push_back(4);
			Indices.push_back(5);

			Indices.push_back(0);
			Indices.push_back(3);
			Indices.push_back(4);
			Indices.push_back(3);
			Indices.push_back(4);
			Indices.push_back(7);

			Indices.push_back(1);
			Indices.push_back(2);
			Indices.push_back(6);
			Indices.push_back(1);
			Indices.push_back(5);
			Indices.push_back(6);

			Vertices.shrink_to_fit();
			Indices.shrink_to_fit();

			// Create GL buffer
			glGenVertexArrays(1, &VAOHandle);
			glBindVertexArray(VAOHandle);

			glGenBuffers(1, &IndexVBOHandle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexVBOHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint16), &Indices[0], GL_STATIC_DRAW);
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
			for (uint32 i = 0; i < Vertices.size(); i++)
			{
				verts.push_back(Vertices.at(i).x);
				verts.push_back(Vertices.at(i).y);
				verts.push_back(Vertices.at(i).z);
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
			if (EnviromentMap != nullptr)
			{
				EnviromentMap->Destroy();
				delete EnviromentMap;
			}
		}

	};
};