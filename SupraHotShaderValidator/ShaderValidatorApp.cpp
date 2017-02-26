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
#include <ShaderParser.h>
#include <StringUtil.h>

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
		// We need to work with absolute file paths
		Utils::FileSystem::GetInstance()->SetRootPath("");
		window = new Window();
		window->Init(width, height, title);
		window->SetClearColor(0.0f, 0.75f, 0.0f, 1.0f);
		glfwSwapInterval(0);
		SetMainApp(this);
	}

	void ShaderValidatorApp::Resize(uint32 width, uint32 height)
	{
	}

	void ShaderValidatorApp::Render()
	{
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
		CheckJson();
		CheckVertexShader();
		CheckPixelShader();
	}

	void ShaderValidatorApp::Destroy()
	{
		// MaterialCache::GetInstance()->Destroy();
		// TextureCache::GetInstance()->Destroy();
		App::Destroy();
	}

	void ShaderValidatorApp::SetFileListener(std::string shaderDescriptionFileName)
	{
		ShaderDescriptionFileName = shaderDescriptionFileName;
		SHF_PRINTF("Listening for changes in %s \n", ShaderDescriptionFileName.c_str());
	}

	void ShaderValidatorApp::Recompile()
	{
		// Read json.
		// Need to grab the vertex & pixel shader paths and dates.

		std::string fileName = Utils::StringUtil::GetFileNameFromPath(ShaderDescriptionFileName);
		std::string rootPath = Utils::StringUtil::GetPathFromFilePath(ShaderDescriptionFileName);

		Utils::FileSystem::GetInstance()->SetRootPath(rootPath);

		ShaderLibrary::GetInstance()->Destroy();
		ShaderDescription* shaderDescription = ShaderParser::GetInstance()->Parse(fileName);
		bool didCompile = ShaderLibrary::GetInstance()->ProcessShaderDescription(shaderDescription);
		
		if (didCompile)
		{
			window->SetClearColor(0.0f, 0.75f, 0.0f, 1.0f);
		} 
		else
		{
			window->SetClearColor(0.75f, 0.0f, 0.0f, 1.0f);
		}
		
		std::string vertexShaderPath = shaderDescription->VertexShaderPath;
		std::string pixelShaderPath = shaderDescription->PixelShaderPath;

		VertexShaderFilePath = rootPath + vertexShaderPath;
		PixelShaderFilePath = rootPath + pixelShaderPath;

		VertexShaderDate = CheckFileTime(VertexShaderFilePath);
		PixelShaderDate = CheckFileTime(PixelShaderFilePath);
	}

	void ShaderValidatorApp::CheckJson()
	{
		FILETIME currentShaderDescriptionDate = CheckFileTime(ShaderDescriptionFileName);

		if (currentShaderDescriptionDate.dwLowDateTime != ShaderDescriptionDate.dwLowDateTime)
		{
			printf("Recompiling shader, because description changed. \n");
			ShaderDescriptionDate = currentShaderDescriptionDate;
			Recompile();
		}

	}

	void ShaderValidatorApp::CheckVertexShader()
	{
		FILETIME currentVertexShaderDate = CheckFileTime(VertexShaderFilePath);

		if (currentVertexShaderDate.dwLowDateTime != VertexShaderDate.dwLowDateTime)
		{
			printf("Recompiling shader, because vertex shader changed. \n");
			Recompile();
		}
	}

	void ShaderValidatorApp::CheckPixelShader()
	{
		FILETIME currentPixelShaderDate = CheckFileTime(PixelShaderFilePath);;

		if (currentPixelShaderDate.dwLowDateTime != PixelShaderDate.dwLowDateTime)
		{
			printf("Recompiling shader, because pixel shader changed. \n");
			Recompile();
		}
	}

	FILETIME ShaderValidatorApp::CheckFileTime(std::string pathToFile)
	{
		WIN32_FIND_DATA findFileData;
		HANDLE h = FindFirstFile(pathToFile.c_str(), &findFileData);
		return findFileData.ftLastWriteTime;
	}
};