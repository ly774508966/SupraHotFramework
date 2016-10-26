#pragma once
#include "Platform.h"

namespace SupraHot {
	class Controls{
	public:
		static bool isKeyDown(int keycode);
		static bool isKeyUp(int keycode);
		static bool isKeyPressed(int keycode);
		static bool isMouseButtonDown(int keycode);
		static bool isMouseButtonUp(int keycode);
		static bool isMouseButtonPressed(int keycode);
		static void update(Graphics::WindowWin32* window);
	};
};
