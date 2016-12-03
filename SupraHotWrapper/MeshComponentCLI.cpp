#include "MeshComponentCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		MeshComponentCLI::MeshComponentCLI()
		{
			Instance = new SupraHot::MeshComponent(nullptr, nullptr);
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
			SupraHot::Graphics::Material* material = meshComponent->GetMaterial();

			SupraHot::CLI::Material^ mat = gcnew SupraHot::CLI::Material();
			mat->IsCopy = true;
			mat->ReplaceInstance(material);

			return mat;
		}
	};
};