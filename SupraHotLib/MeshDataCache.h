#pragma once
#include "Platform.h"
#include "MeshData.h"
#include <vector>
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class MeshDataCache
		{
		private:
			MeshDataCache();

			std::unordered_map<std::string, std::vector<MeshDataPtr>> CachedMeshData;
		public:
			static MeshDataCache* GetInstance();
			~MeshDataCache();

			void Cache(std::string meshName, std::vector<MeshData*> meshes);
			bool IsCached(std::string meshName);
			std::vector<MeshDataPtr>* GetCachedMeshData(std::string modelFilePath);
			MeshDataPtr GetCachedMeshData(std::string modelFilePath, uint32 index);
			void Free(std::string meshName, uint32 index, MeshDataPtr& meshDataPtr);
			void Destroy();
		};
	};
};