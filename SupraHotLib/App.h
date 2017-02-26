#pragma once
#include "Platform.h"
#include "Timer.h"

namespace SupraHot
{
	using namespace Graphics;

	class App
	{
	
	protected:
		Window* window;
		Timer Timer;
		float Time = 0.0f;
		float UpdateTick = 1.0f / 60.0f;
		float ProcessedTime = 0.0f;
		uint32 FPS = 0;

	public:
		App();
		virtual ~App();

		virtual void Init(uint32 width, uint32 height, std::string title);
		virtual void Resize(uint32 width, uint32 height);
		virtual void Render();
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
		virtual void Tick(float deltaTime);
		virtual void Destroy();

		void Run();
		void ProgressApp();
		void Loaded();
		//void PostRender();
		static App* GetMainApp();
		static void SetMainApp(App* app);
		uint32 GetFPS();
	};
};

