#include "Platform.h"
#include "SandBoxApp.h"
#include <FileSystem.h>
#include <iostream>
#include "Shader.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#include "stdafx.h"
#include <tchar.h>
#include <Controls.h>
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "WindowEmscripten.h"
#endif

#include <MeshDataLoader.h>
#include <LuaVM.h>
#include <SHFMBinaryLoader.h>
#include "Platform.h"
#include <TextureCube.h>
#include <SkyBox.h>
#include <MeshDataRenderer.h>

using namespace SupraHot;

SandBoxApp::SandBoxApp()
{
}

SandBoxApp::~SandBoxApp()
{
}

void SandBoxApp::Init(SupraHot::uint32 width, SupraHot::uint32 height, std::string title)
{
	// First we need to initialize the filesystem
#ifdef PLATFORM_WINDOWS
	Utils::FileSystem::GetInstance()->SetRootPath("../Content/");
#endif

#ifdef PLATFORM_EMSCRIPTEN
	Utils::FileSystem::GetInstance()->SetRootPath("Content/");
#endif

	App::Init(width, height, title);

	window = new Window();
	window->Init(width, height, title);
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	FBO = new FrameBufferObject();
	FBO->Init(width, height);

	Texture = new Texture2D("SHF Logo");
	Texture->Load("Images/logo.png");
	FBO->SetReadSource(Texture);

	// Load Shaders
	FBOShader = new Shader();
	SimpleMeshShader = new Shader();
	SkyBoxCubeShader = new Shader();
	SkyBoxSphereShader = new Shader();

#ifdef PLATFORM_WINDOWS
	FBOShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/fbo.vs.glsl");
	FBOShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/fbo.fs.glsl");
	
	SkyBoxCubeShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/skybox.vs.glsl");
	SkyBoxCubeShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/skybox.fs.glsl");

	SkyBoxSphereShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/skybox.vs.glsl");
	SkyBoxSphereShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/skybox-sphere.fs.glsl");

	SimpleMeshShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/simple-mesh.vs.glsl");
	SimpleMeshShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/simple-mesh.fs.glsl");
#endif

#ifdef PLATFORM_ANDROID
	FBOShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/fbo_gles3.vs.glsl");
	FBOShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/fbo_gles3.fs.glsl");

	SkyBoxCubeShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/skybox_gles3.vs.glsl");
	SkyBoxCubeShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/skybox_gles3.fs.glsl");

	SkyBoxSphereShader->LoadShaderFromFile(Shader::VERTEX_SHADER, "Shaders/skybox_gles3.vs.glsl");
	SkyBoxSphereShader->LoadShaderFromFile(Shader::PIXEL_SHADER, "Shaders/skybox-sphere_gles3.fs.glsl");
#endif
	
	FBOShader->CompileShader();
	FBO->SetPixelSize(FBOShader);

	SkyBoxCubeShader->CompileShader();
	SkyBoxSphereShader->CompileShader();

	SimpleMeshShader->CompileShader();
	FBO->SetPixelSize(SimpleMeshShader);

	SHF_PRINTF("SHADER COMPILED \n");

	// Try loading a lua script and run it.
	Scripting::LuaVM::GetInstance()->RunFile("Scripts/test.lua");
	
#if 0
	// Try loading shfm
	Utils::SHFModelFile model = Utils::SHFMBinaryLoader::GetInstance().LoadFromFile("Models/Pistol_Model.shfm");
	SHF_PRINTF("(R) Header %s \n", model.Header.c_str());
	SHF_PRINTF("(R) Mesh count : %d \n", model.MeshCount);
	SHF_PRINTF("(R) Material count : %d \n", model.MaterialCount);
	SHF_PRINTF("(R) Footer %s \n", model.Footer.c_str());

	for (uint32 i = 0; i < model.MaterialCount; i++)
	{
		Utils::SHFModel::Material& material = model.Materials[i];
		SHF_PRINTF("(R) Material name: %s \n", material.Name.c_str());
		SHF_PRINTF("(R) Material albedo: %s \n", material.AlbedoMapPath.c_str());
		SHF_PRINTF("(R) Material normal map: %s \n", material.NormalMapPath.c_str());
		SHF_PRINTF("(R) Material specular: %s \n", material.SpecularMapPath.c_str());
		SHF_PRINTF("- - - - - - - - \n");
	}
#endif

	MeshDataVector = Utils::MeshDataLoader::GetInstance()->Load("Models/Pistol_Model.shfm");
	//SHF_PRINTF("MeshDataVector.size = %llu \n", MeshDataVector.size());

	// Try to load a 2d .dds file
	Texture2D* ddsTexture = new Texture2D("DDS Test");
	ddsTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	ddsTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	ddsTexture->Load("Textures/test/lion_128f.dds");
	FBO->SetReadSource(ddsTexture);
	
	TextureCube* textureCube = new TextureCube("CubeTexture Test");
	textureCube->SetWrapS(GL_CLAMP_TO_EDGE);
	textureCube->SetWrapT(GL_CLAMP_TO_EDGE);
	textureCube->SetWrapR(GL_CLAMP_TO_EDGE);
	textureCube->Load("Textures/Bridge2/px.png", "Textures/Bridge2/nx.png",
					  "Textures/Bridge2/py.png", "Textures/Bridge2/ny.png",
					  "Textures/Bridge2/pz.png", "Textures/Bridge2/nz.png");

	TextureCube* ddsCubeTexture = new TextureCube("DDS Cube map");
	ddsCubeTexture->SetWrapS(GL_CLAMP_TO_EDGE);
	ddsCubeTexture->SetWrapT(GL_CLAMP_TO_EDGE);
	ddsCubeTexture->SetWrapR(GL_CLAMP_TO_EDGE);

	//ddsCubeTexture->LoadDDS("Textures/Random/miptest.dds", false, false);
	//ddsCubeTexture->LoadDDS("Textures/MonValley_G_DirtRoad_3k/Diffuse.dds", true);
	ddsCubeTexture->LoadDDS("Textures/MonValley_G_DirtRoad_3k/Specular.dds", true, true);
	//ddsCubeTexture->LoadDDS("Textures/Random/miptest.dds", false);

	Texture2D* sphereMap = new Texture2D("sphere map");
	sphereMap->SetWrapS(GL_CLAMP_TO_EDGE);
	sphereMap->SetWrapT(GL_CLAMP_TO_EDGE);
	sphereMap->Load("Textures/MonValley_G_DirtRoad_3k/Static.dds");
	//sphereMap->Destroy();

	EnvBox = new SkyBox(); 
	EnvBox->SetEnvironmentMap(ddsCubeTexture);
	//EnvBox->SetEnvironmentMap(sphereMap);
	EnvBox->Init(); 
	
	FlyCamera = new Camera(50.0f, 0.05f, 100.0f, static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight()));
}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
#if PLATFORM_ANDROID
	window->Resize(width, height);
#endif

	FBO->Resize(width, height);
	FlyCamera->AspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void SandBoxApp::Render()
{
	FBO->Attach();

	EnvBox->Render(FlyCamera, SkyBoxCubeShader);

	for (MeshData* meshData : MeshDataVector)
	{
		MeshDataRenderer::GetInstance().Render(FlyCamera, meshData, SimpleMeshShader);
	}
	
	FBO->Detach();
	FBO->SetReadSource(FBO->GetColorRenderTarget());

	FBO->RenderToScreen(FBOShader);
}

void SandBoxApp::Update(float deltaTime)
{
	deltaTime = 0.016f;

	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	FlyCamera->ResetMatrices();
	FlyCamera->Update(deltaTime);
	
#if PLATFORM_WINDOWS
	Controls::GetInstance()->Update(window);

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetForward() * deltaTime;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetBack() * deltaTime;
	}

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetLeft() * deltaTime;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetRight() * deltaTime;
	}

	if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_SPACE))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetUp() * deltaTime;
	}
	else if (Controls::GetInstance()->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		FlyCamera->Position += FlyCamera->GetQuaternion().GetDown() * deltaTime;
	}


#endif
}

void SandBoxApp::LateUpdate(float deltaTime)
{
}

void SandBoxApp::Tick(float deltaTime)
{
}

void SandBoxApp::Destroy()
{
	Texture->Destroy();
	FBOShader->Destroy();
	FBO->Destroy();
	window->Destroy();
}