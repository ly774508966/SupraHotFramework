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

		void MeshDataCache::Cache(std::string meshName, std::vector<MeshData*> meshes)
		{
			if (CachedMeshData.find(meshName) == CachedMeshData.end())
			{
				std::vector<MeshDataPtr> meshDataPtrs;

				for (MeshData* meshData : meshes)
				{
					meshDataPtrs.push_back(MeshDataPtr(meshData));
				}

				CachedMeshData[meshName] = meshDataPtrs;
			}
		}

		void MeshDataCache::Free(std::string meshName, uint32 index, MeshDataPtr& meshDataPtr)
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("MeshDataCache::Free(%s)\n", meshName.c_str());
#endif
			if (meshDataPtr.get() != nullptr && meshDataPtr.use_count() <= 2 && IsCached(meshName))
			{
				// TODO: Freeing code.
				
				// TextureCubes.erase(TextureCubes.find(textureCube->GetPath()));

			}
		}

		bool MeshDataCache::IsCached(std::string meshName)
		{
			return CachedMeshData.find(meshName) != CachedMeshData.end();
		}

		std::vector<MeshDataPtr>* MeshDataCache::GetCachedMeshData(std::string meshName)
		{
			if (CachedMeshData.find(meshName) != CachedMeshData.end())
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("GetCachedMeshData(%s) \n", meshName.c_str());
#endif
				return &CachedMeshData.at(meshName);
			}

			return nullptr;
		}

		void MeshDataCache::Destroy()
		{
			CachedMeshData.clear();
		}
	};
};