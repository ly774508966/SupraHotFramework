#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Math
	{
		class Mat4;
	}

	namespace Graphics
	{
		class ViewFrustum
		{
		private:

		public:
			ViewFrustum();
			~ViewFrustum();
			void Calculate(Math::Mat4 inverseViewProjectionMatrix);
		};
	};
};