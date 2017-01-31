#include "MaterialCache.h"

namespace SupraHot
{
	namespace Graphics
	{
		MaterialCache* MaterialCache::GetInstance()
		{
			static MaterialCache* Instance(new MaterialCache);
			return Instance;
		}

		MaterialCache::MaterialCache()
		{
		}

		MaterialCache::~MaterialCache()
		{
		}

		void MaterialCache::Init()
		{
				{
					ShaderDescription* shaderDescription = ShaderLibrary::GetInstance()->GetShaderDescriptions()->at("MeshDefaultShader");
					MaterialInputs* materialInputs = new MaterialInputs(shaderDescription);
					MeshDefaultMaterial = MaterialInputsPtr(materialInputs);
				}

				{
					ShaderDescription* shaderDescription = ShaderLibrary::GetInstance()->GetShaderDescriptions()->at("MeshBasicShader");
					MaterialInputs* materialInputs = new MaterialInputs(shaderDescription);
					MeshBasicMaterial = MaterialInputsPtr(materialInputs);
				}
		}

		void MaterialCache::Cache(MaterialInputsPtr material)
		{
			if (material.get() != nullptr && CachedMaterials.find(material->GetMaterialFilePath()) == CachedMaterials.end())
			{
				CachedMaterials[material->GetMaterialFilePath()] = material;
			}
		}

		void MaterialCache::Free(MaterialInputsPtr& material)
		{
			SHF_PRINTF("(FreeMaterial) Texture count = [%d] \n", material.use_count());

			if (material.get() != nullptr && material.use_count() <= 2 && CachedMaterials.find(material->GetMaterialFilePath()) != CachedMaterials.end())
			{
				CachedMaterials.erase(CachedMaterials.find(material->GetMaterialFilePath()));
			}
		}

		bool MaterialCache::IsCached(std::string materialFilePath)
		{
			return CachedMaterials.find(materialFilePath) != CachedMaterials.end();;
		}

		MaterialInputsPtr MaterialCache::GetCached(std::string materialFilePath)
		{
			return CachedMaterials[materialFilePath];
		}

		void MaterialCache::Destroy()
		{
			typedef std::unordered_map<std::string, MaterialInputsPtr>::iterator it_type;
			for (it_type iterator = CachedMaterials.begin(); iterator != CachedMaterials.end(); ++iterator)
			{
				iterator->second.get()->Destroy();
			}

			CachedMaterials.clear();

			MeshDefaultMaterial->Destroy();
			MeshBasicMaterial->Destroy();

			SHF_PRINTF("MaterialCache::Destroy()\n");
		}

		MaterialInputsPtr& MaterialCache::GetMeshDefaultMaterial()
		{
			return MeshDefaultMaterial;
		}

		MaterialInputsPtr& MaterialCache::GetMeshBasicMaterial()
		{
			return MeshBasicMaterial;
		}
	};
};