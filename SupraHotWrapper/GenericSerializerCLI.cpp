#include "GenericSerializerCLI.h"
#include <msclr\marshal_cppstd.h>
#include <ShaderLibrary.h>
#include <ShaderMaterial.h>

namespace SupraHot
{
	namespace CLI
	{
		GenericSerializerCLI::GenericSerializerCLI(System::String^ pathToFile)
		{
			Instance = new SupraHot::Utils::GenericSerializer(msclr::interop::marshal_as<std::string>(pathToFile));
		}


		void GenericSerializerCLI::Serialize(EntityCLI^ entity)
		{
			Instance->Serialize(entity->GetHandle());
			SHF_PRINTF(" GenericSerializerCLI::Serialize \n ");
		}

		EntityCLI^ GenericSerializerCLI::Deserialize()
		{
			SupraHot::Entity* entity = new SupraHot::Entity();
			Instance->Deserialize(entity);

			SupraHot::CLI::EntityCLI^ entityCLI = gcnew SupraHot::CLI::EntityCLI();
			entityCLI->ReplaceInstance(entity);
			SetIsCopy(entityCLI);
			return entityCLI;
		}

		Material^ GenericSerializerCLI::DeserialzeMaterial()
		{
			SupraHot::Graphics::ShaderMaterial* shaderMaterial = new SupraHot::Graphics::ShaderMaterial();
			Instance->Deserialize(shaderMaterial);
			return gcnew Material(shaderMaterial);
		}

		void GenericSerializerCLI::Serialize(Material^ material)
		{
			Instance->Serialize(material->GetHandle());
		}

		void GenericSerializerCLI::SetIsCopy(EntityCLI^ entity)
		{
			entity->IsCopy = true;

			auto children = entity->GetChildren();
			
			for (uint32 i = 0, l = children->Count; i < l; ++i)
			{
				SetIsCopy(children[i]);
			}
		}

		void GenericSerializerCLI::SerializeEmptyMaterial(System::String^ shaderName, System::String^ materialName)
		{
			SupraHot::Graphics::ShaderDescription* shaderDescription = SupraHot::Graphics::ShaderLibrary::GetInstance()->GetShaderDescriptions()->at(msclr::interop::marshal_as<std::string>(shaderName));
			
			if (shaderDescription != nullptr)
			{
				SupraHot::Graphics::ShaderMaterial* emptyMaterial = new SupraHot::Graphics::ShaderMaterial(shaderDescription);
				emptyMaterial->Name = msclr::interop::marshal_as<std::string>(materialName);
				GetHandle()->Serialize(emptyMaterial);
				delete emptyMaterial;
			}
		}
	};
};