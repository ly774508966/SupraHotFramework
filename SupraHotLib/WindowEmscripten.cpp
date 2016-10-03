#include "WindowEmscripten.h"
#include "App.h"

#ifdef PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>
#endif


namespace SupraHot
{
	namespace Graphics
	{

#ifdef PLATFORM_EMSCRIPTEN
		static void dispatch()
		{
			App::GetMainApp()->ProgressApp();
		}
#endif

		static std::vector<WindowEmscripten*> instances;

		void OnWindowResized(GLFWwindow* window, int width, int height)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			// TODO: Throws error on maximize.
			int err = glGetError();
			if (err != 0)
			{
				printf("Error %d after resize to %d x %d \n", err, width, height);
			}

			printf("Global resize FN called \n");

			for (size_t i = 0, l = instances.size(); i < l; ++i)
			{
				if (instances.at(i)->GetGLFWWindow() == window)
				{
					instances.at(i)->Resize(width, height);
					break;
				}
			}
		}

		WindowEmscripten::WindowEmscripten()
		{
		}

		WindowEmscripten::~WindowEmscripten()
		{
		}

		void WindowEmscripten::Setup()
		{
			if (!glfwInit())
			{
				printf("Error! Could not initialize GLFW!");
			}

			int OpenGLMajor = 3;
			int OpenGLMinor = 0;

			// Setup OpenGL Params
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinor);

			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Set Colorbits
			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);
			glfwWindowHint(GLFW_STENCIL_BITS, 8);
			glfwWindowHint(GLFW_DEPTH_BITS, 24);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		}

		void WindowEmscripten::Init(uint32 width, uint32 height, std::string title)
		{
			instances.erase(std::remove(instances.begin(), instances.end(), this), instances.end());
			instances.push_back(this);

			this->Width = width;
			this->Height = height;
			this->WindowTitle = title;

			Setup();
			GlfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

			if (!GlfwWindow)
			{
				printf("GLFW Window could not be created! \n");
			}

			// Setup OpenGL Parameters
			glfwMakeContextCurrent(GlfwWindow);

			// Setup Callbacks
			glfwSetWindowSizeCallback(GlfwWindow, OnWindowResized);

			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				std::cout << "OpenGL Error: " << error << std::endl;
			}

			int err = glGetError();
			if (err != 0)
			{
				printf("Error %d AFTER INIT \n", err);
			}


#ifdef PLATFORM_EMSCRIPTEN
			emscripten_set_main_loop(dispatch, 0, 1);
#endif
		}

		void WindowEmscripten::Resize(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;

			printf("window resized to: %d x %d \n", width, height);

			App::GetMainApp()->Resize(width, height);
		}

		bool WindowEmscripten::ShouldClose()
		{
			return glfwWindowShouldClose(GlfwWindow) == 1;
		}

		void WindowEmscripten::Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(GlfwWindow);
		}

		void WindowEmscripten::Destroy()
		{
			glfwTerminate();
		}

		void WindowEmscripten::Clear()
		{
			glClearColor(ClearColorR, ClearColorG, ClearColorB, ClearColorA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};
};