#include "Material.h"
#include "MaterialCache.h"
#include "MurmurHash.h"

namespace SupraHot
{
	namespace Graphics
	{
		Material::Material(MaterialInputsPtr materialInputs)
		{
			MaterialInputs = materialInputs;

			// Create the uuid here.			
			GenerateUUID();
		}

		Material::~Material()
		{
			MaterialCache::GetInstance()->Free(MaterialInputs);
		}

		void Material::GenerateUUID()
		{
			std::string toHash = MaterialInputs->GetName();
			Utils::MurmurHash::Hash hashed = Utils::MurmurHash::GenerateHash(toHash.data(), int(toHash.length()), 0);
			UUID = hashed.A;
		}

		void Material::UpdateShaderPermutation(MeshDataPtr& meshData)
		{
			Shader* shader = ShaderLibrary::GetInstance()->SelectShaderForShaderMaterialAndMeshData(meshData.get(), MaterialInputs.get());
			ShaderPermutation = shader;
			MaterialInputs->Update(shader);
		}

		Shader* Material::GetShader()
		{
			return ShaderPermutation;
		}

		MaterialInputsPtr& Material::GetMaterialInputs()
		{
			return MaterialInputs;
		}

		uint64 Material::GetUUID()
		{
			return UUID;
		}
	};
};