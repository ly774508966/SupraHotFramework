#pragma once
#include "Wrapper.h"
#include <WindowEditorWin32.h>

namespace SupraHot
{
	namespace Graphics
	{
		public ref class WindowEditor : public Wrapper<WindowEditorWin32>
		{
		public:
			void SetHWND(void* hwnd);
			void Init(uint32 width, uint32 height);
			void Update();
			void Clear();
			void SetClearColor(float r, float g, float b, float a);
		};
	};
};
