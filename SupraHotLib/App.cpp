#include "App.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#endif

namespace SupraHot
{
	App::App()
	{
	}

	App::~App()
	{
	}

	void App::Init(uint32 width, uint32 height, std::string title)
	{
		printf("Init \n");
	}

	void App::Resize(uint32 width, uint32 height)
	{
	}

	void App::Render()
	{
	}

	void App::Update(float deltaTime)
	{
	}

	void App::LateUpdate(float deltaTime)
	{
	}

	void App::Tick(float deltaTime)
	{
	}

	void App::Destroy()
	{
	}

	void App::Run()
	{
		while (true && !window->ShouldClose())
		{
			glViewport(0, 0, window->GetWidth(), window->GetHeight());

			window->Clear();

			Render();

			window->Update();
		}
	}
};
