#include "ShaderMaterial.h"
#include "ShaderLibrary.h"
#include "ShaderDescription.h"
#include "Vec3MaterialProperty.h"
#include "BooleanMaterialProperty.h"
#include "Vec2MaterialProperty.h"
#include "Vec4MaterialProperty.h"
#include "FloatMaterialProperty.h"

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
			this->Description = shaderDescription;
			this->Name = shaderDescription->Name + " Material";


			// todo: this is just for testing. these parameters should be only generated in the editor!
			typedef std::unordered_map<std::string, std::string>::iterator it;
			for (it iterator = shaderDescription->Uniforms.begin(); iterator != shaderDescription->Uniforms.end(); ++iterator)
			{
				std::string name = iterator->first;
				std::string type = iterator->second;

				// todo: need to do an "isInternal" check...
				if (type == "float")
				{
					AddMaterialProperty(new FloatMaterialProperty(name));
				}
				else if (type == "bool")
				{
					AddMaterialProperty(new BooleanMaterialProperty(name));
				}
				else if (type == "vec2")
				{
					AddMaterialProperty(new Vec2MaterialProperty(name));
				}
				else if (type == "vec3")
				{
					AddMaterialProperty(new Vec3MaterialProperty(name));
				}
				else if (type == "vec4")
				{
					AddMaterialProperty(new Vec4MaterialProperty(name));
				}
				else if (type == "texture2D")
				{

				}
				else if (type == "textureCube")
				{

				}

			}
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
	};
};