#include "App.h"

#ifdef PLATFORM_ANDROID
#include "WindowAndroid.h"
#endif

#ifdef PLATFORM_WINDOWS
#include "WindowWin32.h"
#endif

#ifdef PLATFORM_EMSCRIPTEN
#include "WindowEmscripten.h"
#endif
#include "ShaderLibrary.h"

namespace SupraHot
{
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

	uint32 App::GetFPS()
	{
		return FPS;
	}

	void App::Init(uint32 width, uint32 height, std::string title)
	{
		MainApp = this;
		ShaderLibrary::GetInstance()->Initialize();
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
		ShaderLibrary::GetInstance()->Destroy();
	}

	// Note: The Run()-method should only be used on desktops to keep the window open!
	// On platforms like Android and Emscripten, we just need to call the ProgressApp()-Method in their
	// lifecycle.
	void App::Run()
	{
		while (!window->ShouldClose())
		{
			ProgressApp();
		}
	}

	void App::ProgressApp()
	{
		// TODO: Create a proper game loop here
		glViewport(0, 0, window->GetWidth(), window->GetHeight());
		
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 0, window->GetWidth(), window->GetHeight());

		window->Clear();

		Timer.Update();
		float deltaTime = Timer.DeltaSecondsF();
		float elapsed = Timer.ElapsedSecondsF();
		
		//while (ProcessedTime < UpdateTick)
		{
			Update(deltaTime);
			LateUpdate(deltaTime);

		//	ProcessedTime += deltaTime;
			//printf("PT %f && < %d \n", ProcessedTime, (ProcessedTime < UpdateTick));
		} 
		//ProcessedTime = 0;
		
		{
			// should Probally measure the amount of rendering time.
			Render();
			window->Update();
			glDisable(GL_SCISSOR_TEST);
			FPS++;
		}
		
		if (elapsed - Time > 1.0f)
		{
			Time += 1.0f;
			Tick(deltaTime);
			
			// SHF_PRINTF("fps: %d \n", FPS);
			FPS = 0;
		}
	}

	void App::Loaded()
	{
		Timer.Update();
	}
};
