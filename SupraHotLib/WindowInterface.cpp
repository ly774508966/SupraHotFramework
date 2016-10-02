#include "WindowInterface.h"


namespace SupraHot
{
	namespace Graphics
	{
		WindowInterface::WindowInterface()
		{
			
		}

		WindowInterface::~WindowInterface()
		{
		}

		void WindowInterface::SetClearColor(float r, float g, float b, float a)
		{
			this->ClearColorR = r;
			this->ClearColorG = g;
			this->ClearColorB = b;
			this->ClearColorA = a;
		}
	};
};
