#include "WindowAndroid.h"
#include "App.h"

namespace SupraHot
{
	namespace Graphics
	{
		void WindowAndroid::Setup()
		{
			// Note: This is handled by the Java-Class
		}

		void WindowAndroid::Init(uint32 width, uint32 height, std::string title)
		{
			// Note: This is handled by the Java-Class
		}

		void WindowAndroid::Resize(uint32 width, uint32 height)
		{
			// Note: The acutall resize of the "window" is handled by the Java-Class.
			// We just need to resize the Main-FrameBufferObject

			this->Width = width;
			this->Height = height;

			glViewport(0, 0, width, height);
			SHF_PRINTF("window resized to: %d x %d \n", width, height);
		}

		bool WindowAndroid::ShouldClose()
		{
			// Note: This should be set by the Java-Class

			return false;
		}

		void WindowAndroid::Update()
		{
			// Poll events
		}

		void WindowAndroid::Destroy()
		{
			// This is handled by the Java-Class itself
		}

		void WindowAndroid::Clear()
		{
			glClearColor(ClearColorR, ClearColorG, ClearColorB, ClearColorA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		WindowAndroid::WindowAndroid()
		{
		}

		WindowAndroid::~WindowAndroid()
		{
		}
	};
};