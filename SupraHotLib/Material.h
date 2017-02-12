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
			uint64 UUID = 0;

			void GenerateUUID();
		public:
			// Members
			bool Transparent = false;

			// Constructors
			Material(MaterialInputsPtr materialInputs);
			~Material();

			// Methods
			void UpdateShaderPermutation(MeshDataPtr& meshData);
			Shader* GetShader();
			MaterialInputsPtr& GetMaterialInputs();
			uint64 GetUUID();
		}; 
	};
};