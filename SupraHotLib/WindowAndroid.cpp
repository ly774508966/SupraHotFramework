#include "WindowAndroid.h"

namespace SupraHot
{
	namespace Graphics
	{
		void WindowAndroid::Setup()
		{
		}

		void WindowAndroid::Init(uint32 width, uint32 height, std::string title)
		{
		}

		void WindowAndroid::Resize(uint32 width, uint32 height)
		{
		}

		bool WindowAndroid::ShouldClose()
		{
			return false;
		}

		void WindowAndroid::Update()
		{
		}

		void WindowAndroid::Destroy()
		{
		}

		void WindowAndroid::Clear()
		{
			glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		WindowAndroid::WindowAndroid()
		{
		}

		WindowAndroid::~WindowAndroid()
		{
		}
	};
};