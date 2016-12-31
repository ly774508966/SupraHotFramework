#pragma once
#include "Platform.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Utils
	{
		class MaterialLoader
		{
		private:
			MaterialLoader();
		public:
			static MaterialLoader* GetInstance();
			~MaterialLoader();
		};
	};
};

