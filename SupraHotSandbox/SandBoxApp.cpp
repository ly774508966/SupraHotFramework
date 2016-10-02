#include "SandBoxApp.h"
#include "Platform.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
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
	App::Init(width, height, title);
	window = new SupraHot::Window();
	window->Init(width, height, title);

	window->SetClearColor(0, 1, 0, 1);
}

void SandBoxApp::Resize(SupraHot::uint32 width, SupraHot::uint32 height)
{
}

void SandBoxApp::Render()
{
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