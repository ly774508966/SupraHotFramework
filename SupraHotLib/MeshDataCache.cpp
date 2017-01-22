#include "MeshDataCache.h"
#include "MeshDataLoader.h"

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
			SHF_PRINTF("MeshDataCache::Free(%s, %d)\n", meshName.c_str(), index);
#endif
			if (meshDataPtr.get() != nullptr && meshDataPtr.use_count() <= 2 && IsCached(meshName))
			{
				std::vector<MeshDataPtr>* cachedMesh = &CachedMeshData.at(meshName);
				cachedMesh->erase(cachedMesh->begin() + index);
				cachedMesh->insert(cachedMesh->begin() + index, nullptr);
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

		MeshDataPtr MeshDataCache::GetCachedMeshData(std::string meshName, uint32 index)
		{
			if (CachedMeshData.find(meshName) != CachedMeshData.end())
			{
#if DEVELOPMENT == 1
				SHF_PRINTF("GetCachedMeshData(%s, %d) \n", meshName.c_str(), index);
#endif

				// Check if MeshData at index is valid

				if (CachedMeshData.at(meshName).at(index) != nullptr)
				{
					return CachedMeshData.at(meshName).at(index);
				}
				
				// if not, load the file and cached it.

				std::vector<MeshData*> rawMeshData = Utils::MeshDataLoader::GetInstance()->LoadRawData(meshName);
				if (rawMeshData.size() <= index)
				{
					SHF_PRINTF("Could not find any valid MeshData in %s at index %d \n", meshName.c_str(), index);
					return nullptr;
				}

				MeshData* meshData = rawMeshData.at(index);
				rawMeshData.erase(rawMeshData.begin() + index);

				for (MeshData* mesh : rawMeshData)
				{
					mesh->Destroy();
					delete mesh;
				} rawMeshData.clear();

				
				CachedMeshData.at(meshName).erase(CachedMeshData.at(meshName).begin() + index);
				CachedMeshData.at(meshName).insert(CachedMeshData.at(meshName).begin() + index, MeshDataPtr(meshData));
			}

			return nullptr;
		}

		void MeshDataCache::Destroy()
		{
			CachedMeshData.clear();
		}
	};
};