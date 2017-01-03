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

		MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::MeshData^ meshData, SupraHot::CLI::Material^ material)
		{
			Instance = new SupraHot::MeshComponent(meshData->GetHandle(), material->GetHandle());
		}

		MeshComponentCLI::MeshComponentCLI(SupraHot::CLI::ComponentCLI^ componentCLI)
		{
			Instance = componentCLI->GetHandle();
		}

		SupraHot::CLI::Material^ MeshComponentCLI::GetMaterial()
		{

			SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
			SupraHot::Graphics::ShaderMaterial* material = meshComponent->GetMaterial();

			SupraHot::CLI::Material^ mat = gcnew SupraHot::CLI::Material();
			mat->IsCopy = true;
			mat->ReplaceInstance(material);

			return mat;
		}

		void MeshComponentCLI::UpdateShaderPermuation()
		{
			SupraHot::MeshComponent* meshComponent = static_cast<SupraHot::MeshComponent*>(Instance);
			meshComponent->UpdateShaderPermution();
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
	};
};