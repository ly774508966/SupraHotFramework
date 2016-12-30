#include "ShaderMaterial.h"
#include "ShaderLibrary.h"
#include "ShaderDescription.h"
#include "Vec3MaterialProperty.h"
#include "BooleanMaterialProperty.h"
#include "Vec2MaterialProperty.h"
#include "Vec4MaterialProperty.h"
#include "FloatMaterialProperty.h"
#include "Texture2DMaterialProperty.h"
#include "TextureCubeMaterialProperty.h"
#include <algorithm>

namespace SupraHot
{
	namespace Graphics
	{
		ShaderMaterial::ShaderMaterial()
		{
			this->Name = "NO MATERIAL";
		}

		ShaderMaterial::ShaderMaterial(ShaderDescription* shaderDescription)
		{
			SetShaderDescription(shaderDescription);
		}

		ShaderMaterial::~ShaderMaterial()
		{

		}

		bool ShaderMaterial::AddMaterialProperty(MaterialProperty* materialProperty)
		{
			// check if the material property is not already present in the vector
			if (std::find(MaterialProperties.begin(), MaterialProperties.end(), materialProperty) == MaterialProperties.end())
			{
				MaterialProperties.push_back(materialProperty);
				SelectShaderPermutation();
				return true;
			}

			return false;
		}

		bool ShaderMaterial::RemoveMaterialProperty(MaterialProperty* materialProperty)
		{
			if (std::find(MaterialProperties.begin(), MaterialProperties.end(), materialProperty) != MaterialProperties.end())
			{
				MaterialProperties.erase(std::remove(MaterialProperties.begin(), MaterialProperties.end(), materialProperty), MaterialProperties.end());
				SelectShaderPermutation();
				delete materialProperty;
				return true;
			}

			return false;
		}

		bool ShaderMaterial::RemoveAllMaterialProperties()
		{
			// Loop through all mps and delete them.
			for (uint32 i = 0, l = static_cast<uint32>(MaterialProperties.size()); i < l; ++i)
			{
				delete MaterialProperties.at(i);
			}

			MaterialProperties.clear();
			return true;
		}

		MaterialProperty* ShaderMaterial::GetMaterialPropertyByName(std::string materialPropertyName)
		{
			for (MaterialProperty* materialProperty : MaterialProperties)
			{

				if (materialProperty->GetName() == materialPropertyName)
				{
					return materialProperty;
				}
			}

			return nullptr;
		}

		ShaderDescription* ShaderMaterial::GetShaderDescription()
		{
			return Description;
		}

		std::vector<MaterialProperty*>* ShaderMaterial::GetMaterialProperties()
		{
			return &MaterialProperties;
		}

		Shader* ShaderMaterial::GetShader()
		{
			return ShaderPermutation;
		}

		void ShaderMaterial::Update()
		{
			if (this->ShaderPermutation != nullptr)
			{
				this->ShaderPermutation->Attach();
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->SetLocation(this->ShaderPermutation);
				}

				// Note: if we want to use this inside the mesh renderer, this line should be removed
				this->ShaderPermutation->Detach();
			}
		}

		void ShaderMaterial::Apply()
		{
			if (this->ShaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->Apply(this->ShaderPermutation);
				}
			}
		}

		void ShaderMaterial::Unbind()
		{
			if (this->ShaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->Unbind(this->ShaderPermutation);
				}
			}
		}

		void ShaderMaterial::Destroy()
		{
			SHF_PRINTF("Todo: ShaderMaterial::Destroy() \n");
			// shader descriptions get deleted by shaderlibrary!
		}

		void ShaderMaterial::SelectShaderPermutation()
		{

		}

		void ShaderMaterial::SetShaderPermutation(Shader* shader)
		{
			this->ShaderPermutation = shader;
			Update();
		}

		void ShaderMaterial::SetShaderDescription(ShaderDescription* shaderDescription)
		{
			this->Description = shaderDescription;
			this->Name = shaderDescription->Name + " Material";
		}
	};
};