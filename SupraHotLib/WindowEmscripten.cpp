#include "WindowEmscripten.h"
#include "App.h"

namespace SupraHot
{
	namespace Graphics
	{
		WindowEmscripten::WindowEmscripten()
		{
		}

		WindowEmscripten::~WindowEmscripten()
		{
		}

		void WindowEmscripten::Setup()
		{
		}

		void WindowEmscripten::Init(uint32 width, uint32 height, std::string title)
		{
		}

		void WindowEmscripten::Resize(uint32 width, uint32 height)
		{
			// Note: The acutall resize of the "window" is handled by the Java-Class.
			// We just need to resize the Main-FrameBufferObject

			this->Width = width;
			this->Height = height;

			printf("window resized to: %d x %d \n", width, height);

			// Note: This should be called directly from the Java-Class
			App::GetMainApp()->Resize(width, height);
		}

		bool WindowEmscripten::ShouldClose()
		{
			return false;
		}

		void WindowEmscripten::Update()
		{
		}

		void WindowEmscripten::Destroy()
		{
		}

		void WindowEmscripten::Clear()
		{
			glClearColor(ClearColorR, ClearColorG, ClearColorB, ClearColorA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};
};