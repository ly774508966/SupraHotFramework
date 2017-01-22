#include "MeshComponentCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		MeshComponentCLI::MeshComponentCLI()
		{
			Instance = new SupraHot::MeshComponent(nullptr, nullptr);
		}

		MeshComponentCLI::~MeshComponentCLI()
		{
#if DEVELOPMENT == 1
			SHF_PRINTF("~MeshComponentCLI \n");
#endif
		}

	/*	MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::MeshData^ meshData, SupraHot::CLI::Material^ material)
		{
			Instance = new SupraHot::MeshComponent(meshData->GetHandle(), material->GetHandle());

			Vec3 origin = meshData->GetHandle()->Origin;
			Origin = gcnew Vec3CLI(origin.x, origin.y, origin.z);
		}*/

		MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::ComponentCLI^ componentCLI)
		{
			Instance = componentCLI->GetHandle();
		}

		SupraHot::CLI::Material^ MeshComponentCLI::GetMaterial()
		{
			if (Material == nullptr)
			{
				SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
				SupraHot::Graphics::ShaderMaterial* material = meshComponent->GetMaterial();

				Material = gcnew SupraHot::CLI::Material();
				Material->IsCopy = true;
				Material->ReplaceInstance(material);
				Material->CheckExistingMaterialProperties();
			}

			return Material;
		}

		void MeshComponentCLI::UpdateShaderPermuation()
		{
			SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
			meshComponent->UpdateShaderPermution();
		}

		System::String^ MeshComponentCLI::GetMeshName()
		{
			return msclr::interop::marshal_as<System::String^>(static_cast<SupraHot::MeshComponent*>(Instance)->GetMeshData()->Name);
		}

		unsigned int MeshComponentCLI::GetModelFileArrayIndex()
		{
			return static_cast<SupraHot::MeshComponent*>(Instance)->GetModelFileArrayIndex();
		}

		System::String^ MeshComponentCLI::GetModelFilePath()
		{
			return msclr::interop::marshal_as<System::String^>(static_cast<SupraHot::MeshComponent*>(Instance)->GetModelFilePath());
		}

		bool MeshComponentCLI::SetShader(System::String^ shaderName)
		{
			// ShaderName is 100% valid at this point, since it is being selected in the gui.
			std::string shaderNameStd = msclr::interop::marshal_as<std::string>(shaderName);
			SupraHot::Graphics::ShaderDescription* shaderDescription = SupraHot::Graphics::ShaderLibrary::GetInstance()->GetShaderDescriptions()->at(shaderNameStd);
			static_cast<SupraHot::MeshComponent*>(Instance)->ChangeShader(shaderDescription);
#if DEVELOPMENT == 1
			SHF_PRINTF("Shader description file name: %s \n", shaderDescription->FileName.c_str());
#endif
			return true;
		}

		Vec3CLI^ MeshComponentCLI::GetMeshDataOrigin()
		{
			return Origin;
		}
	};
};