#pragma once
#include "Platform.h"
#include "Shader.h"
#include "MaterialInputs.h"
#include "MeshData.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Material
		{
		private:
			MaterialInputsPtr MaterialInputs;
			Shader* ShaderPermutation;
		public:
			Material(MaterialInputsPtr materialInputs);
			~Material();

			void UpdateShaderPermutation(MeshDataPtr& meshData);
			Shader* GetShader();
			MaterialInputsPtr& GetMaterialInputs();
		}; 
	};
};