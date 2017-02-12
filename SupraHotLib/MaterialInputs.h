#pragma once
#include "Platform.h"
#include "Shader.h"
#include "MaterialProperty.h"
#include "ShaderDescription.h"
#include <memory>

namespace SupraHot
{
	namespace Graphics
	{
		class MaterialInputs
		{
		private:
			std::string MaterialFilePath;
			ShaderDescription* Description;
			std::vector<MaterialProperty*> MaterialProperties;
			bool HasChanged = true;

			std::string Name;
		public:
			// Todo: remove this default construcotr & do some clever stuff in the CLI version
			MaterialInputs();
			MaterialInputs(ShaderDescription* shaderDescription);
			~MaterialInputs();

			bool AddMaterialProperty(MaterialProperty* materialProperty);
			bool RemoveMaterialProperty(MaterialProperty* materialProperty);
			bool RemoveAllMaterialProperties();
			MaterialProperty* GetMaterialPropertyByName(std::string materialPropertyName);
			ShaderDescription* GetShaderDescription();
			std::vector<MaterialProperty*>* GetMaterialProperties();

			void Update(Shader* shaderPermutation);
			void Apply(Shader* shaderPermutation);
			void Unbind(Shader* shaderPermutation);
			void Destroy();
			
			void SetShaderDescription(ShaderDescription* shaderDescription);

			std::string GetMaterialFilePath();
			void SetMaterialFilePath(std::string filePath);

			void SetName(std::string name);
			std::string GetName();

			void Updated();
			bool NeedsUpdate();
		};

		typedef std::shared_ptr<MaterialInputs> MaterialInputsPtr;
	};
};