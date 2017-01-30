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
			Vec3CLI^ Origin;
		public:
			MeshComponentCLI();
			MeshComponentCLI(SupraHot::CLI::ComponentCLI^ componentCLI);
			//MeshComponentCLI(SupraHot::CLI::MeshData^ meshData, SupraHot::CLI::Material^ material);
			~MeshComponentCLI();

			SupraHot::CLI::Material^ GetMaterial();
			System::String^ GetMeshName();

			unsigned int GetModelFileArrayIndex();
			System::String^ GetModelFilePath();
			
			void UpdateShaderPermuation();
			bool SetShader(System::String^ shaderName);
			void SetMaterial(System::String^ shaderName);
			void SetMaterial(SupraHot::CLI::Material^ material);

			Vec3CLI^ GetMeshDataOrigin();
		};
	};
};