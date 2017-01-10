#pragma once
#include "ComponentCLI.h"
#include "MaterialCLI.h"
#include "MeshDataCLI.h"
#include <MeshComponent.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshComponentCLI : SupraHot::CLI::ComponentCLI
		{
		private:
			SupraHot::CLI::Material^ Material = nullptr;
		public:
			MeshComponentCLI();
			~MeshComponentCLI();
			MeshComponentCLI(SupraHot::CLI::ComponentCLI^ componentCLI);
			MeshComponentCLI(SupraHot::CLI::MeshData^ meshData, SupraHot::CLI::Material^ material);

			SupraHot::CLI::Material^ GetMaterial();
			
			void UpdateShaderPermuation();
			bool SetShader(System::String^ shaderName);
		};
	};
};