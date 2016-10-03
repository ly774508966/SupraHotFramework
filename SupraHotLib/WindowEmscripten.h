#pragma once
#include "Platform.h"
#include "WindowInterface.h"

namespace SupraHot
{
	namespace Graphics
	{
		class WindowEmscripten : public WindowInterface
		{
		private:
			GLFWwindow* GlfwWindow;
		protected:
			void Setup() override;
		public:
			WindowEmscripten();
			~WindowEmscripten();

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