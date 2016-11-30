#include "WindowEditorWin32.h"
#include "App.h"

namespace SupraHot
{
	namespace Graphics
	{
		static PIXELFORMATDESCRIPTOR GetPixelFormat()
		{
			PIXELFORMATDESCRIPTOR result = {};
			result.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			result.nVersion = 1;
			result.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			result.iPixelType = PFD_TYPE_RGBA;
			result.cColorBits = 32;
			result.cDepthBits = 32;
			result.cStencilBits = 8;
			result.cAuxBuffers = 0;
			result.iLayerType = PFD_MAIN_PLANE;
			return result;
		}

		WindowEditorWin32::WindowEditorWin32()
		{
		}


		WindowEditorWin32::~WindowEditorWin32()
		{
		}

		void WindowEditorWin32::Setup()
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		}

		void WindowEditorWin32::SetHWND(HWND hwnd)
		{
			this->Hwnd = hwnd;
		}

		void WindowEditorWin32::Init(uint32 width, uint32 height, std::string title)
		{
			this->Width = width;
			this->Height = height;
			this->WindowTitle = title;


			// use hwnd and hook into it

			if (!Hwnd)
			{
				SHF_PRINTF("Could create window editor win32. Missing hwnd.\nDid you call window.SetHWND()?\n");
				return;
			}

			Hdc = GetDC(Hwnd);
			if (!Hdc) {
				SHF_PRINTF("Could create window editor win32. Missing Hdc.\nDid you call window.SetHWND()?\n");
				return;
			}

			PIXELFORMATDESCRIPTOR pfd = GetPixelFormat();
			int32 pixelFormat = ChoosePixelFormat(Hdc, &pfd);
			if (pixelFormat)
			{
				if (!SetPixelFormat(Hdc, pixelFormat, &pfd))
				{
					SHF_PRINTF("Failed setting pixel format!\n");
					return;
				}
			}
			else
			{
				SHF_PRINTF("Failed choosing pixel format!\n");
				return;
			}


			HGLRC hrc = wglCreateContext(Hdc);
			if (hrc)
			{
				if (!wglMakeCurrent(Hdc, hrc))
				{
					SHF_PRINTF("Failed setting OpenGL context!\n");
				}
			}
			else
			{
				SHF_PRINTF("Failed creating OpenGL context!!\n");
			}

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
			{
				SHF_PRINTF("Could not initialize GLEW!!\n");
			}

			Setup();

		}

		void WindowEditorWin32::Resize(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;

			SHF_PRINTF("Internal window resized to: %d x %d \n", width, height);
			glViewport(0, 0, width, height);
		}

		bool WindowEditorWin32::ShouldClose()
		{
			return false;
		}

		void WindowEditorWin32::Update()
		{
			SwapBuffers(Hdc);
		}

		void WindowEditorWin32::Destroy()
		{
			SHF_PRINTF("Destroy!\n");
		}

		void WindowEditorWin32::Clear()
		{
			glClearColor(ClearColorR, ClearColorG, ClearColorB, ClearColorA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};
};
