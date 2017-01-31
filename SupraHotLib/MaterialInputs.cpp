#include "MaterialInputs.h"
#include "ShaderDescription.h"
#include <algorithm>

namespace SupraHot
{
	namespace Graphics
	{
		MaterialInputs::MaterialInputs()
		{
			this->Name = "NO MATERIAL";
		}

		MaterialInputs::MaterialInputs(ShaderDescription* shaderDescription)
		{
			SetShaderDescription(shaderDescription);
		}

		MaterialInputs::~MaterialInputs()
		{
			Destroy();
		}

		bool MaterialInputs::AddMaterialProperty(MaterialProperty* materialProperty)
		{
			// check if the material property is not already present in the vector
			if (std::find(MaterialProperties.begin(), MaterialProperties.end(), materialProperty) == MaterialProperties.end())
			{
				MaterialProperties.push_back(materialProperty);
				HasChanged = true;
				return true;
			}

			return false;
		}

		bool MaterialInputs::RemoveMaterialProperty(MaterialProperty* materialProperty)
		{
			if (std::find(MaterialProperties.begin(), MaterialProperties.end(), materialProperty) != MaterialProperties.end())
			{
				MaterialProperties.erase(std::remove(MaterialProperties.begin(), MaterialProperties.end(), materialProperty), MaterialProperties.end());
				delete materialProperty;
				HasChanged = true;
				return true;
			}

			return false;
		}

		bool MaterialInputs::RemoveAllMaterialProperties()
		{
			// Loop through all mps and delete them.
			for (uint32 i = 0, l = static_cast<uint32>(MaterialProperties.size()); i < l; ++i)
			{
				delete MaterialProperties.at(i);
			}
			HasChanged = true;

			MaterialProperties.clear();
			return true;
		}

		MaterialProperty* MaterialInputs::GetMaterialPropertyByName(std::string materialPropertyName)
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

		ShaderDescription* MaterialInputs::GetShaderDescription()
		{
			return Description;
		}

		std::vector<MaterialProperty*>* MaterialInputs::GetMaterialProperties()
		{
			return &MaterialProperties;
		}

		void MaterialInputs::Update(Shader* shaderPermutation)
		{
			if (shaderPermutation != nullptr)
			{
				shaderPermutation->Attach();
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->SetLocation(shaderPermutation);
				}

				// Note: if we want to use this inside the mesh renderer, this line should be removed
				shaderPermutation->Detach();
			}
		}

		void MaterialInputs::Apply(Shader* shaderPermutation)
		{
			if (shaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->Apply(shaderPermutation);
				}
			}
		}

		void MaterialInputs::Unbind(Shader* shaderPermutation)
		{
			if (shaderPermutation != nullptr)
			{
				for (MaterialProperty* materialProperty : MaterialProperties)
				{
					materialProperty->Unbind(shaderPermutation);
				}
			}
		}

		void MaterialInputs::Destroy()
		{
			for (MaterialProperty* materialProperty : MaterialProperties)
			{
				delete materialProperty;
			}
			MaterialProperties.clear();
		}

		void MaterialInputs::SetShaderDescription(ShaderDescription* shaderDescription)
		{
			this->Description = shaderDescription;
			this->Name = shaderDescription->Name + " Material";
		}

		std::string MaterialInputs::GetMaterialFilePath()
		{
			return MaterialFilePath;
		}

		void MaterialInputs::SetMaterialFilePath(std::string filePath)
		{
			MaterialFilePath = filePath;
		}

		void MaterialInputs::Updated()
		{
			HasChanged = false;
		}

		bool MaterialInputs::NeedsUpdate()
		{
			return HasChanged;
		}
	};
};