#include "MaterialCache.h"
#include "GenericSerializer.h"

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
					//ShaderDescription* shaderDescription = ShaderLibrary::GetInstance()->GetShaderDescriptions()->at("MeshDefaultShader");
					//MaterialInputs* materialInputs = new MaterialInputs(shaderDescription);
					//MeshDefaultMaterial = MaterialInputsPtr(materialInputs);
					//MeshDefaultMaterial->SetMaterialFilePath("MeshDefaultMaterial");
					//MeshDefaultMaterial->Name = "MeshDefaultMaterial";
					//CachedMaterials["MeshDefaultMaterial"] = MeshDefaultMaterial;

					LoadIntoCache("Materials/MeshDefaultMaterial.json");

				}

				{
					// ShaderDescription* shaderDescription = ShaderLibrary::GetInstance()->GetShaderDescriptions()->at("MeshBasicShader");
					// MaterialInputs* materialInputs = new MaterialInputs(shaderDescription);
					// MeshBasicMaterial = MaterialInputsPtr(materialInputs);
					// MeshBasicMaterial->Name = "MeshBasicMaterial";
					// CachedMaterials["MeshBasicMaterial"] = MeshBasicMaterial;

					LoadIntoCache("Materials/MeshBasicMaterial.json");
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

		void MaterialCache::LoadIntoCache(std::string materialFilePath)
		{
			MaterialInputs* material = new Graphics::MaterialInputs();
			MaterialInputsPtr materialInputsPtr;

			Utils::GenericSerializer shaderMaterialDeserializer(materialFilePath);
			shaderMaterialDeserializer.Deserialize(material);
			material->SetMaterialFilePath(materialFilePath);

			materialInputsPtr = MaterialInputsPtr(material);
			Cache(materialInputsPtr);
		}

		void MaterialCache::Destroy()
		{
			typedef std::unordered_map<std::string, MaterialInputsPtr>::iterator it_type;
			for (it_type iterator = CachedMaterials.begin(); iterator != CachedMaterials.end(); ++iterator)
			{
				iterator->second.get()->Destroy();
			}

			CachedMaterials.clear();

			SHF_PRINTF("MaterialCache::Destroy()\n");
		}

		MaterialInputsPtr& MaterialCache::GetMeshDefaultMaterial()
		{
			return CachedMaterials["Materials/MeshDefaultMaterial.json"];
		}

		MaterialInputsPtr& MaterialCache::GetMeshBasicMaterial()
		{
			return CachedMaterials["Materials/MeshBasicMaterial.json"];
		}
	};
};