#include "ShaderValidatorApp.h"
#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#include <Controls.h>
#include "stdafx.h"
#include <tchar.h>
#endif

#include <FileSystem.h>
#include <TextureCache.h>
#include <MaterialCache.h>

namespace SupraHot
{
	ShaderValidatorApp::ShaderValidatorApp()
	{
	}

	ShaderValidatorApp::~ShaderValidatorApp()
	{
	}

	void ShaderValidatorApp::Init(uint32 width, uint32 height, std::string title)
	{
		Utils::FileSystem::GetInstance()->SetRootPath("../Content/");
		window = new Window();
		window->Init(width, height, title);
		window->SetClearColor(0.7f, 0.3f, 0.7f, 1.0f);
		glfwSwapInterval(0);

		App::Init(width, height, title);

		MaterialCache::GetInstance()->Init();
		TextureCache::GetInstance()->Init();

		SHF_PRINTF("Listening for changes in %s \n", ShaderDescriptionFileName.c_str());
	}

	void ShaderValidatorApp::Resize(uint32 width, uint32 height)
	{
	}

	void ShaderValidatorApp::Render()
	{
		glClearColor(0, 1, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ShaderValidatorApp::Update(float deltaTime)
	{
	}

	void ShaderValidatorApp::LateUpdate(float deltaTime)
	{
	}

	void ShaderValidatorApp::Tick(float deltaTime)
	{
	}

	void ShaderValidatorApp::Destroy()
	{
		MaterialCache::GetInstance()->Destroy();
		TextureCache::GetInstance()->Destroy();
		App::Destroy();
	}

	void ShaderValidatorApp::SetFileListener(std::string shaderDescriptionFileName)
	{
		ShaderDescriptionFileName = shaderDescriptionFileName;
	}
};