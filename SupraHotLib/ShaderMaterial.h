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
			std::string Name;

			// Todo: remove this default construcotr & do some clever stuff in the CLI version
			ShaderMaterial();
			ShaderMaterial(ShaderDescription* shaderDescription);
			~ShaderMaterial();

			bool AddMaterialProperty(MaterialProperty* materialProperty);
			bool RemoveMaterialProperty(MaterialProperty* materialProperty);
			MaterialProperty* GetMaterialPropertyByName(std::string materialPropertyName);
			ShaderDescription* GetShaderDescription();
			std::vector<MaterialProperty*>* GetMaterialProperties();
			Shader* GetShader();

			void Update();
			void Apply();
			void Destroy();
			
			// todo: Rework this bit.
			// We need the mesh data in order to select a shader from the shaderlib....
			void SelectShaderPermutation();
			void SetShaderPermutation(Shader* shader);
		};
	};
};