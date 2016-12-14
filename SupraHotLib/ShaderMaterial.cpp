#include "ShaderMaterial.h"
#include "ShaderLibrary.h"
#include "ShaderDescription.h"

namespace SupraHot
{
	namespace Graphics
	{
		
		ShaderMaterial::ShaderMaterial(ShaderDescription* shaderDescription)
		{
			this->Description = shaderDescription;
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
			SelectShaderPermutation();
			return false;
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

		void ShaderMaterial::Update()
		{
			if (this->ShaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->SetLocation(this->ShaderPermutation);
				}
			}
		}

		void ShaderMaterial::Attach()
		{
			if (this->ShaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->Apply(this->ShaderPermutation);
				}
			}
		}

		void ShaderMaterial::SelectShaderPermutation()
		{
			
		}
	};
};