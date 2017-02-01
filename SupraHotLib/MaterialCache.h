#pragma once
#include "Platform.h"
#include "MaterialInputs.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class MaterialCache
		{
		private:
			MaterialCache();		
			std::unordered_map<std::string, MaterialInputsPtr> CachedMaterials;
		public:
			static MaterialCache* GetInstance();
			~MaterialCache();
			
			void Init();
			void Cache(MaterialInputsPtr material);
			void Free(MaterialInputsPtr& material);
			bool IsCached(std::string materialFilePath);
			MaterialInputsPtr GetCached(std::string materialFilePath);
			void LoadIntoCache(std::string materialFilePath);
			void Destroy();

			MaterialInputsPtr& GetMeshDefaultMaterial();
			MaterialInputsPtr& GetMeshBasicMaterial();
		};
	};
};