#pragma once
#include "Platform.h"

namespace SupraHot {
	class Controls{
	public:
		static bool IsKeyDown(int keycode);
		static bool IsKeyUp(int keycode);
		static bool IsKeyPressed(int keycode);
		static bool IsMouseButtonDown(int keycode);
		static bool IsMouseButtonUp(int keycode);
		static bool IsMouseButtonPressed(int keycode);
		static void Update(Graphics::WindowWin32* window);
	};
};
