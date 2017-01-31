#include "Material.h"
#include "MaterialCache.h"

namespace SupraHot
{
	namespace Graphics
	{
		Material::Material(MaterialInputsPtr materialInputs)
		{
			MaterialInputs = materialInputs;
		}

		Material::~Material()
		{
			MaterialCache::GetInstance()->Free(MaterialInputs);
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
	};
};