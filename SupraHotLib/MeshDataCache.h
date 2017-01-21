#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class MeshDataCache
		{
		private:
			MeshDataCache();
		public:
			static MeshDataCache* GetInstance();
			~MeshDataCache();
		};
	};
};