#pragma once
#include "Platform.h"

namespace SupraHot 
{
	class Controls
	{
	private:
		Controls();
	public:
		bool IsKeyDown(int keycode);
		bool IsKeyUp(int keycode);
		bool IsKeyPressed(int keycode);
		bool IsMouseButtonDown(int keycode);
		bool IsMouseButtonUp(int keycode);
		bool IsMouseButtonPressed(int keycode);
		void GrabMouse();
		void ReleaseMouse();
		void Update(Graphics::WindowWin32* window);

		float MouseX, MouseY, MouseDX, MouseDY;
		bool MouseGrab = false, MouseRelease = false;

		static Controls* GetInstance();
	};
};
