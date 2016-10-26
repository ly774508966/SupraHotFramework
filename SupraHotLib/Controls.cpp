#include "Controls.h"
#include <vector>
#include <set>
#include "WindowWin32.h"

namespace SupraHot{


	static std::set<int> KEY_DOWN;
	static std::set<int> KEY_PRESSED;
	static std::set<int> KEY_UP;

	static std::set<int> MOUSE_DOWN;
	static std::set<int> MOUSE_PRESSED;
	static std::set<int> MOUSE_UP;

	void Controls::update(Graphics::WindowWin32* window){
		// Fetch all keys !
		// keys -> 32 bis <= 348
		// mouse -> 0 bis <= 7

		KEY_PRESSED.clear();
		KEY_UP.clear();

		MOUSE_PRESSED.clear();
		MOUSE_UP.clear();

		// Loop through all keys and check, if they are pressed

		for (int i = 32; i <= 348; i++){
			if (glfwGetKey(window->GetGLFWWindow(), i)){
				// Contains :
				if (!isKeyDown(i)){
					KEY_DOWN.insert(i);
					KEY_PRESSED.insert(i);
				}
			}
			else{
				if (isKeyDown(i)){
					KEY_UP.insert(i);
					KEY_DOWN.erase(i);
				}
			}
		}

		// Loop trough all mouse buttons and check, if they are pressed

		for (int i = 0; i <= 7; i++){
			if (glfwGetMouseButton(window->GetGLFWWindow(), i)){
				if (!isMouseButtonDown(i)){
					MOUSE_DOWN.insert(i);
					MOUSE_PRESSED.insert(i);
				}
			}
			else{
				if (isMouseButtonDown(i)){
					MOUSE_UP.insert(i);
					MOUSE_DOWN.erase(i);
				}
			}
		}

	}

	bool Controls::isKeyDown(int keycode){
		if (KEY_DOWN.find(keycode) != KEY_DOWN.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Controls::isKeyUp(int keycode){
		if (KEY_UP.find(keycode) != KEY_UP.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Controls::isKeyPressed(int keycode){
		if (KEY_PRESSED.find(keycode) != KEY_PRESSED.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Controls::isMouseButtonDown(int keycode){
		if (MOUSE_DOWN.find(keycode) != MOUSE_DOWN.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Controls::isMouseButtonUp(int keycode){
		if (MOUSE_UP.find(keycode) != MOUSE_UP.end()) {
			return true;
		}
		else {
			return false;
		}
	}

	bool Controls::isMouseButtonPressed(int keycode){
		if (MOUSE_PRESSED.find(keycode) != MOUSE_PRESSED.end()) {
			return true;
		}
		else {
			return false;
		}
	}

};