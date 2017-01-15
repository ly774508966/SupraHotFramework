#include "GenericSerializerCLI.h"
#include <msclr\marshal_cppstd.h>

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
			return entityCLI;
		}
	};
};