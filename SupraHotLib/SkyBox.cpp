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
			shader->SetMat4(glGetUniformLocation(shaderProgramID, "ViewProjectionMatrix"), *camera->GetViewProjectionMatrix());
			shader->SetTextureCube(glGetUniformLocation(shaderProgramID, "CubeMap"), EnviromentMap, 0);

			// Draw the vertices
			Render();

			shader->Detach();

			glEnable(GL_DEPTH_TEST);
			camera->Position = cameraPosition;
		}

		void SkyBox::Render()
		{
			// Bind GL buffers

			// DrawArray.
		}

		void SkyBox::Init()
		{
			// Create box vertices

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

			// Create GL buffers
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