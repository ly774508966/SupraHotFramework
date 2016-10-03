#include "App.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "WindowEmscripten.h"
#include <emscripten/emscripten.h>
#endif

namespace SupraHot
{
#ifdef PLATFORM_EMSCRIPTEN
	static void dispatch()
	{
		App::GetMainApp()->ProgressApp();
	}
#endif

	App::App()
	{
	}

	App::~App()
	{
	}

	// ---------
	// Main App
	// ---------
	static App* MainApp;
	App* App::GetMainApp()
	{
		return MainApp;
	}

	void App::Init(uint32 width, uint32 height, std::string title)
	{
		MainApp = this;

#ifdef PLATFORM_EMSCRIPTEN
		emscripten_set_main_loop(dispatch, 0, 1);
#endif
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

	// Note: The Run()-method should only be used on desktops to keep the window open!
	// On platforms like Android and Emscripten, we just need to call the ProgressApp()-Method in their
	// lifecycle.
	void App::Run()
	{
		while (true && !window->ShouldClose())
		{
			ProgressApp();
		}
	}

	void App::ProgressApp()
	{
		// TODO: Create a proper game loop here
		glViewport(0, 0, window->GetWidth(), window->GetHeight());

		window->Clear();
		
		Update(0.0f);
		LateUpdate(0.0f);
		Tick(0.0f);

		Render();

		window->Update();
	}
};
