#include "WindowWin32.h"
#include <iostream>
#include <algorithm>
#include "App.h"

namespace SupraHot
{
	namespace Graphics
	{
		static std::vector<WindowWin32*> instances;

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

		WindowWin32::WindowWin32()
		{
			
		}

		WindowWin32::~WindowWin32()
		{
		}

		void WindowWin32::Setup()
		{
			if (!glfwInit())
			{
				printf("Error! Could not initialize GLFW!");
			}

			int OpenGLMajor = 4;
			int OpenGLMinor = 0;

			// Setup OpenGL Params
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OpenGLMajor);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OpenGLMinor);

			//glfwWindowHint(GLFW_OPENGL_PROFILE, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			// Set Colorbits
			glfwWindowHint(GLFW_RED_BITS, 8);
			glfwWindowHint(GLFW_GREEN_BITS, 8);
			glfwWindowHint(GLFW_BLUE_BITS, 8);
			glfwWindowHint(GLFW_ALPHA_BITS, 8);
			glfwWindowHint(GLFW_STENCIL_BITS, 8);
			glfwWindowHint(GLFW_DEPTH_BITS, 32);
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		}

		void WindowWin32::PrintVersion(GLFWwindow* context)
		{
			std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "GL Vendor: " << glGetString(GL_VENDOR) << std::endl;

			int major, minor, rev;
			major = glfwGetWindowAttrib(context, GLFW_CONTEXT_VERSION_MAJOR);
			minor = glfwGetWindowAttrib(context, GLFW_CONTEXT_VERSION_MINOR);
			rev = glfwGetWindowAttrib(context, GLFW_CONTEXT_REVISION);
			printf("OpenGL version recieved: %d.%d.%d \n", major, minor, rev);

			int r, g, b, a;
			r = glfwGetWindowAttrib(context, GLFW_RED_BITS);
			g = glfwGetWindowAttrib(context, GLFW_GREEN_BITS);
			b = glfwGetWindowAttrib(context, GLFW_BLUE_BITS);
			a = glfwGetWindowAttrib(context, GLFW_ALPHA_BITS);
			printf("RGBA: %d %d %d %d \n", r, g, b, a);

			std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

			GLint temp;
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &temp);
			printf("Max. Texture Units: %d \n", temp);
		}

		void WindowWin32::Init(uint32 width, uint32 height, std::string title)
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

			glewExperimental = GL_TRUE;
			if (glewInit() != GLEW_OK)
			{
				printf("Error! Could not initialize GLEW!");
			}

			PrintVersion(GlfwWindow);
			int err = glGetError();
			if (err != 0)
			{
				printf("Error %d AFTER INIT \n", err);
			}
		}

		void WindowWin32::Resize(uint32 width, uint32 height)
		{
			this->Width = width;
			this->Height = height;

			printf("window resized to: %d x %d \n", width, height);

			App::GetMainApp()->Resize(width, height);
		}

		bool WindowWin32::ShouldClose()
		{
			return glfwWindowShouldClose(GlfwWindow) == 1;
		}

		void WindowWin32::Update()
		{
			glfwPollEvents();
			glfwSwapBuffers(GlfwWindow);
		}

		void WindowWin32::Destroy()
		{
			glfwTerminate();
		}

		void WindowWin32::Clear()
		{
			glClearColor(ClearColorR, ClearColorG, ClearColorB, ClearColorA);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};
};
