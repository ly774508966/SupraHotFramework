#pragma once
#include "Platform.h"

namespace SupraHot
{
	using namespace Graphics;

	class App
	{
	protected:
		Window* window;
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
	};
};

