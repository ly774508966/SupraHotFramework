#pragma once
#include "Platform.h"
#include "ShaderMaterial.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Graphics
	{
		class MaterialCache
		{
		private:
			MaterialCache();		
			std::unordered_map<std::string, MaterialPtr> CachedMaterials;
		public:
			static MaterialCache* GetInstance();
			~MaterialCache();
			
			void Init();
			void Cache(MaterialPtr material);
			void Free(MaterialPtr& material);
			bool IsCached(std::string materialFilePath);
			MaterialPtr GetCached(std::string materialFilePath);
			void Destroy();
		};
	};
};