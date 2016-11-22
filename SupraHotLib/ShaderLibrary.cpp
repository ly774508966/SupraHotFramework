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

		Shader* ShaderLibrary::SelectShaderForMaterialAndMeshData(Graphics::MeshData* meshData, Graphics::Material* material)
		{
			return nullptr;
		}

		void ShaderLibrary::Initialize()
		{
			std::string directoryPath = "Shaders/GL/";

#ifdef PLATFORM_ANDROID
			directoryPath = "Shaders/GLES3/";
#endif


			// Init skybox shaders
			{
				Shader* skyboxCubeShader = new Shader();
				skyboxCubeShader->SetName("Skybox Cubemap Shader");
				skyboxCubeShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "skybox.vs.glsl");
				skyboxCubeShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "skybox.fs.glsl");
				skyboxCubeShader->CompileShader();
				Skybox[uint32(SkyboxShader::CubeMap)] = skyboxCubeShader;


				Shader* skyboxSphereShader = new Shader();
				skyboxSphereShader->SetName("Skybox Sphere Shader");
				skyboxSphereShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "skybox.vs.glsl");
				skyboxSphereShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "skybox-sphere.fs.glsl");
				skyboxSphereShader->CompileShader();
				Skybox[uint32(SkyboxShader::SphereMap)] = skyboxSphereShader;
			}

			// Screen space shaders
			{
				Shader* fboShader = new Shader();
				fboShader->SetName("Render to screen");
				fboShader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "fbo.vs.glsl");
				fboShader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "fbo.fs.glsl");
				fboShader->CompileShader();
				ScreenSpace[uint32(ScreenSpace::RenderToScreen)] = fboShader;

			}

			// Load mesh vertex shaders
			{
				ShaderCompileOptions opts;
				
				// - - - - - - - - - - - - - - - - - - - - - - -
				// Position
				// - - - - - - - - - - - - - - - - - - - - - - -
				
				opts.Define("_Normals", false);
				opts.Define("_UV", false);
				opts.Define("_Tangents", false);
				
				Shader* meshShaderPosition = new Shader();
				meshShaderPosition->SetName("Mesh (Position)");
				meshShaderPosition->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
				meshShaderPosition->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
				meshShaderPosition->CompileShader();
				MeshStatic[uint32(StaticMesh::VertexShader::Position)] = meshShaderPosition;

				// - - - - - - - - - - - - - - - - - - - - - - -
				// Position | Normal
				// - - - - - - - - - - - - - - - - - - - - - - -

				opts.Reset();
				opts.Define("_Normals", true);
				opts.Define("_UV", false);
				opts.Define("_Tangents", false);

				Shader* meshShaderPositionNormal = new Shader();
				meshShaderPositionNormal->SetName("Mesh (Position | Normal)");
				meshShaderPositionNormal->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
				meshShaderPositionNormal->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
				meshShaderPositionNormal->CompileShader();
				MeshStatic[uint32(StaticMesh::VertexShader::PositionNormal)] = meshShaderPositionNormal;

				// - - - - - - - - - - - - - - - - - - - - - - -
				// Position | UV
				// - - - - - - - - - - - - - - - - - - - - - - -

				opts.Reset();
				opts.Define("_Normals", false);
				opts.Define("_UV", true);
				opts.Define("_Tangents", false);

				Shader* meshShaderPositionUV = new Shader();
				meshShaderPositionUV->SetName("Mesh (Position | UV)");
				meshShaderPositionUV->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
				meshShaderPositionUV->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
				meshShaderPositionUV->CompileShader();
				MeshStatic[uint32(StaticMesh::VertexShader::PositionUV)] = meshShaderPositionUV;

				// - - - - - - - - - - - - - - - - - - - - - - -
				// Position | Normal | UV
				// - - - - - - - - - - - - - - - - - - - - - - -

				opts.Reset();
				opts.Define("_Normals", true);
				opts.Define("_UV", true);
				opts.Define("_Tangents", false);

				Shader* meshShaderPositionNormalUV = new Shader();
				meshShaderPositionNormalUV->SetName("Mesh (Position | Normal | UV)");
				meshShaderPositionNormalUV->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
				meshShaderPositionNormalUV->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
				meshShaderPositionNormalUV->CompileShader();
				MeshStatic[uint32(StaticMesh::VertexShader::PositionNormalUV)] = meshShaderPositionNormalUV;

				// - - - - - - - - - - - - - - - - - - - - - - -
				// Position | Normal | UV | Tangent | BiTangent
				// - - - - - - - - - - - - - - - - - - - - - - -

				opts.Reset();
				opts.Define("_Normals", true);
				opts.Define("_UV", true);
				opts.Define("_Tangents", true);

				Shader* meshShaderPositionNormalUVTangentBiTangent = new Shader();
				meshShaderPositionNormalUVTangentBiTangent->SetName("Mesh (Position | Normal | UV | Tangent | BiTangent)");
				meshShaderPositionNormalUVTangentBiTangent->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
				meshShaderPositionNormalUVTangentBiTangent->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
				meshShaderPositionNormalUVTangentBiTangent->CompileShader();
				MeshStatic[uint32(StaticMesh::VertexShader::PositionNormalUVTangentBiTangent)] = meshShaderPositionNormalUVTangentBiTangent;
			}
		}

		void ShaderLibrary::Destroy()
		{
			for (uint32 i = 0, l = uint32(StaticMesh::VertexShader::Count); i < l; ++i)
			{
				Shader* shader = MeshStatic[i];
				if (shader != nullptr)
				{
					shader->Destroy();
				}
			}

			for (uint32 i = 0, l = uint32(SkyboxShader::Count); i < l; ++i)
			{
				Shader* shader = Skybox[i];
				if (shader != nullptr)
				{
					shader->Destroy();
				}
			}

			for (uint32 i = 0, l = uint32(ScreenSpace::Count); i < l; ++i)
			{
				Shader* shader = ScreenSpace[i];
				if (shader != nullptr)
				{
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