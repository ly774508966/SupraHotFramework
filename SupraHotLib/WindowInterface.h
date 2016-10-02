#pragma once
#include "Platform.h"
#include <string>

namespace SupraHot
{
	class App;

	namespace Graphics
	{
		class WindowInterface
		{
		protected:
			uint32 Width, Height;
			std::string WindowTitle;
			float ClearColorR = 0, ClearColorG = 0, ClearColorB = 0, ClearColorA = 0;
			virtual void Setup() = 0;
		public:
			WindowInterface();
			virtual ~WindowInterface();
			virtual void Init(uint32 width, uint32 height, std::string title) = 0;
			virtual void Resize(uint32 width, uint32 height) = 0;
			virtual bool ShouldClose() = 0;
			virtual void Update() = 0;
			virtual void Destroy() = 0;
			virtual void Clear() = 0;

			uint32 GetWidth() { return Width; };
			uint32 GetHeight() { return Height; };
			void SetClearColor(float r, float g, float b, float a);
		};
	};
};

