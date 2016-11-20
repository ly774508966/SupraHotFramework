#include "ShaderLibrary.h"
#include <iostream>

namespace SupraHot
{
	namespace Graphics
	{
		ShaderLibrary::ShaderLibrary()
		{
		}

		ShaderLibrary::~ShaderLibrary()
		{
		}

		void ShaderLibrary::Initialize()
		{
			// Init skybox shaders
			{
				Shader* skyboxCubeShader = new Shader();
				skyboxCubeShader->SetName("Skybox Cubemap Shader");
				skyboxCubeShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/GL/skybox.vs.glsl");
				skyboxCubeShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/GL/skybox.fs.glsl");
				skyboxCubeShader->CompileShader();
				Skybox[SkyboxShader::CubeMap] = skyboxCubeShader;


				Shader* skyboxSphereShader = new Shader();
				skyboxSphereShader->SetName("Skybox Sphere Shader");
				skyboxSphereShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/GL/skybox.vs.glsl");
				skyboxSphereShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/GL/skybox-sphere.fs.glsl");
				skyboxSphereShader->CompileShader();
				Skybox[SkyboxShader::SphereMap] = skyboxSphereShader;
			}

			// Load mesh vertex shaders
		}

		void ShaderLibrary::Destroy()
		{
			for (uint32 i = 0, l = VertexShader::StaticMesh::Count; i < l; ++i)
			{
				Shader* shader = MeshStatic[i];
				if (shader != nullptr)
				{
					shader->Destroy();
				}
			}

			for (uint32 i = 0, l = SkyboxShader::Count; i < l; ++i)
			{
				Shader* shader = Skybox[i];
				if (shader != nullptr)
				{
					SHF_PRINTF("Destroy Shader : %s | \n", shader->GetName().c_str());
					shader->Destroy();
				}
			}
		}

		ShaderLibrary* ShaderLibrary::GetInstance()
		{
			static ShaderLibrary* instance(new ShaderLibrary);
			return instance;
		}

	};
};