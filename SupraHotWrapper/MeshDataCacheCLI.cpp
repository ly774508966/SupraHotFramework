#include "MeshDataCacheCLI.h"

namespace SupraHot
{
	namespace CLI
	{

		void MeshDataCacheCLI::ClearCache()
		{
			SupraHot::Graphics::MeshDataCache::GetInstance()->Destroy();
		}
	
	};
};