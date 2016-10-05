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

	// Try loading objx
	std::vector<SupraHot::MeshData*> meshData = SupraHot::Utils::MeshDataLoader::GetInstance()->Load("Models/cube.mh", SupraHot::Utils::MeshDataLoader::ModelFileFormat::OBJX);
	SHF_PRINTF("MeshData Facecount: %d \n", meshData.at(0)->FaceCount);
	SHF_PRINTF("Material Name %s \n", meshData.at(0)->MeshMaterial.GetName().c_str());
	SHF_PRINTF("Material Albedo Tex ID %d \n", meshData.at(0)->MeshMaterial.GetAlbedoMap()->GetID());

	// Try loading a model file with assimp
	std::vector<SupraHot::MeshData*> meshDataAssimp = SupraHot::Utils::MeshDataLoader::GetInstance()->LoadWithAssimp("Models/cube.obj");
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