#include "Platform.h"
#include "SandBoxApp.h"
#include <FileSystem.h>
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

	App::Init(width, height, title);

	window = new SupraHot::Window();
	window->Init(width, height, title);
	window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);

	FBO = new SupraHot::FrameBufferObject();
	FBO->Init(width, height);

	Texture = new SupraHot::Texture2D("FBO Texture");
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
}

void SandBoxApp::LateUpdate(float deltaTime)
{
}

void SandBoxApp::Tick(float deltaTime)
{
}

void SandBoxApp::Destroy()
{
}