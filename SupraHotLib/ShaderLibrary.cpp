#include "ShaderLibrary.h"
#include <iostream>
#include "Utility.h"

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
			return MeshStatic[uint32(StaticMesh::VertexShader::PositionUV)];
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

			{
				ShaderCompileOptions opts;

				uint32 shaderInputVertexAttribs = 3;
				uint32 shaderInputTextures = 6;
				
				for (uint32 v = 0, vl = static_cast<uint32>(pow(2, shaderInputVertexAttribs)); v < vl; v++)
				{
					std::vector<bool> vertexAttribBools = Utils::Utility::GetBoolCombinations(v, shaderInputVertexAttribs);

					// if we have tangents but no normals, we can skip this shader
					if (vertexAttribBools[2] && !vertexAttribBools[0])
					{
						continue;
					}
	
					for (uint32 t = 0, tl = static_cast<uint32>(pow(2, shaderInputTextures)); t < tl; t++)
					{
						bool compileShader = true;

						opts.Define("_Normals", vertexAttribBools[0]);
						opts.Define("_UV", vertexAttribBools[1]);
						opts.Define("_Tangents", vertexAttribBools[2]);

						std::vector<bool> textureBools = Utils::Utility::GetBoolCombinations(t, shaderInputTextures);
						opts.Define("_AlbedoMap", textureBools[0]);
						opts.Define("_NormalMap", textureBools[1]);
						opts.Define("_RoughnessMap", textureBools[2]);
						opts.Define("_MetalnessMap", textureBools[3]);
						opts.Define("_SpecularMap", textureBools[4]);
						opts.Define("_ComboMap", textureBools[5]);


						// if we run into problems, we can comment this section out and load ALL possible permutations
						// an then just skip the ones, which aren't compiling. LUL.
	
						if (textureBools[1] && !vertexAttribBools[0]) // NormalMap set to true && Normals set to false
						{
							compileShader = false;
						}
						else if (textureBools[1] && !vertexAttribBools[2]) // NormalMap set to true && Tangents set to false
						{
							compileShader = false;
						}

						if (!vertexAttribBools[1]) // no UV's
						{
							if (   textureBools[0] 
								|| textureBools[1]
								|| textureBools[2]
								|| textureBools[3]
								|| textureBools[4]
								|| textureBools[5])
							{
								compileShader = false;
							}
							
						}

						if (compileShader)
						{
							// Load shader with compile options.
							Shader* shader = new Shader();
							shader->SetName("Mesh (....)");
							shader->LoadShaderFromFile(Shader::VERTEX_SHADER, directoryPath + "mesh.vs.glsl", opts);
							shader->LoadShaderFromFile(Shader::PIXEL_SHADER, directoryPath + "mesh.fs.glsl", opts);
							shader->CompileShader();
							MeshShaders.push_back(shader);
						}

						opts.Reset();
					}
				}

				SHF_PRINTF("Created %llu shader permutations \n", MeshShaders.size());				
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
					delete shader;
				}
			}

			for (uint32 i = 0, l = uint32(SkyboxShader::Count); i < l; ++i)
			{
				Shader* shader = Skybox[i];
				if (shader != nullptr)
				{
					shader->Destroy();
					delete shader;
				}
			}

			for (uint32 i = 0, l = uint32(ScreenSpace::Count); i < l; ++i)
			{
				Shader* shader = ScreenSpace[i];
				if (shader != nullptr)
				{
					shader->Destroy();
					delete shader;
				}
			}

			for (uint32 i = 0, l = uint32(MeshShaders.size()); i < l; ++i)
			{
				MeshShaders[i]->Destroy();
				delete MeshShaders[i];
			}
		}

		ShaderLibrary* ShaderLibrary::GetInstance()
		{
			static ShaderLibrary* instance(new ShaderLibrary);
			return instance;
		}

	};
};