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
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "WindowEmscripten.h"
#endif
#include <MeshDataLoader.h>
#include <LuaVM.h>
#include <SHFMBinaryLoader.h>
#include "Platform.h"
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
	SupraHot::Utils::FileSystem::GetInstance()->SetRootPath("../Content/");
#endif

#ifdef PLATFORM_EMSCRIPTEN
	SupraHot::Utils::FileSystem::GetInstance()->SetRootPath("Content/");
#endif

	App::Init(width, height, title);

	window = new SupraHot::Window();
	window->Init(width, height, title);
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	FBO = new SupraHot::FrameBufferObject();
	FBO->Init(width, height);

	Texture = new SupraHot::Texture2D("Pepe Texture");
	Texture->Load("Images/test.png");

	FBO->SetReadSource(Texture);

	// Load Shaders
	FBOShader = new SupraHot::Shader();
#ifdef PLATFORM_WINDOWS
	FBOShader->LoadShaderFromFile(SupraHot::Shader::VERTEX_SHADER, "Shaders/fbo.vs");
	FBOShader->LoadShaderFromFile(SupraHot::Shader::PIXEL_SHADER, "Shaders/fbo.fs");
#endif

#ifdef PLATFORM_ANDROID
	FBOShader->LoadShaderFromFile(SupraHot::Shader::VERTEX_SHADER, "Shaders/fbo_gles3.vs");
	FBOShader->LoadShaderFromFile(SupraHot::Shader::PIXEL_SHADER, "Shaders/fbo_gles3.fs");
#endif

	FBOShader->CompileShader();
	FBO->SetPixelSize(FBOShader);

	// Try loading a lua script and run it.
	SupraHot::Scripting::LuaVM::GetInstance()->RunFile("Scripts/test.lua");

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


	std::vector<MeshData*> meshData = Utils::MeshDataLoader::GetInstance()->Load("Models/Pistol_Model.shfm");

	// Todo: Write .dds-Texture loader.
	// Need this for 2D textures, but also for dds-cubes with mip maps!

	// Try to load a 2d .dds file

	Texture2D* ddsTexture = new Texture2D("DDS Test");
	//ddsTexture->Load("Textures/test/Cerberus_A.dds");
	ddsTexture->Load("Textures/test/lion_bgr.dds");
	// ddsTexture.Load("Textures/test/GraceCathedral.dds");
	FBO->SetReadSource(ddsTexture);
}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
	FBO->Resize(width, height);
}

void SandBoxApp::Render()
{
	FBO->Attach();
	FBO->Detach();
	FBO->RenderToScreen(FBOShader);
}

void SandBoxApp::Update(float deltaTime)
{
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);
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