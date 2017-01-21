#include "MeshDataCache.h"

namespace SupraHot
{
	namespace Graphics
	{
		MeshDataCache::MeshDataCache()
		{
		}

		MeshDataCache* MeshDataCache::GetInstance()
		{
			static MeshDataCache* Instance(new MeshDataCache);
			return Instance;
		}

		MeshDataCache::~MeshDataCache()
		{
		}
	};
};