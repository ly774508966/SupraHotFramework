#pragma once
#include "Platform.h"
#include "WindowInterface.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		class WindowWin32 : public Graphics::WindowInterface
		{
		private:
			GLFWwindow* GlfwWindow;
			void Setup() override;
			void PrintVersion(GLFWwindow* context);
		public:
			WindowWin32(App* app);
			~WindowWin32();
			void Init(uint32 width, uint32 height, std::string title) override;
			void Resize(uint32 width, uint32 height) override;
			bool ShouldClose() override;
			void Update() override;
			void Destroy() override;
			void Clear() override;

			GLFWwindow* GetGLFWWindow() { return GlfwWindow; };
		};
	};
};

