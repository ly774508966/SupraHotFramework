#pragma once
#include "WindowInterface.h"
#include <Windows.h>
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class WindowEditorWin32 : public Graphics::WindowInterface
		{
		private:
			HWND Hwnd;
			HDC  Hdc;
			HINSTANCE hInstance;

			// Gets replaced by constructor
			int glVersionMajor = 0;
			int glVersionMinor = 0;
		protected:
			void Setup() override;
		public:
			void SetHWND(HWND hwnd);
			void Init(uint32 width, uint32 height, std::string title) override;
			void Resize(uint32 width, uint32 height) override;
			bool ShouldClose() override;
			void Update() override;
			void Destroy() override;
			void Clear() override;
			void PrintVersion();
			
			WindowEditorWin32();
			~WindowEditorWin32();
		};
	};
};