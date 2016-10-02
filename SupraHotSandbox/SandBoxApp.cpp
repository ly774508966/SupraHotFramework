#include "Platform.h"
#include "SandBoxApp.h"
#include <FileSystem.h>

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
	window->SetClearColor(0.7, 0.3, 0.2, 1);

	fbo = new SupraHot::FrameBufferObject();
	fbo->Init(width, height);

	texture = new SupraHot::Texture2D("FBO Texture");
	//texture->Load("Images/test.png");

	//texture->Init(width, height);
}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
	fbo->Resize(width, height);
}

void SandBoxApp::Render()
{
	//fbo->Attach();
	//fbo->Detach();
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