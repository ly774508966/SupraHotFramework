#pragma once
#include "Platform.h"
#include "Shader.h"
#include "MaterialProperty.h"
#include "ShaderDescription.h"

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderMaterial
		{
		private:
			Shader* ShaderPermutation;

			ShaderDescription* Description;
			std::vector<MaterialProperty*> MaterialProperties;
		public:
			ShaderMaterial(ShaderDescription* shaderDescription);
			~ShaderMaterial();

			bool AddMaterialProperty(MaterialProperty* materialProperty);
			bool RemoveMaterialProperty(MaterialProperty* materialProperty);
			MaterialProperty* GetMaterialPropertyByName(std::string materialPropertyName);
			ShaderDescription* GetShaderDescription();
			std::vector<MaterialProperty*>* GetMaterialProperties();

			void Update();
			void Attach();
			
			// todo: rework thsi bit
			void SelectShaderPermutation();
		};
	};
};