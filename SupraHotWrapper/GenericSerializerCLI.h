#pragma once
#include <GenericSerializer.h>
#include "EntityCLI.h"
#include "MaterialCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		public ref class GenericSerializerCLI
		{
		private:
			SupraHot::Utils::GenericSerializer* Instance;
		public:
			GenericSerializerCLI(System::String^ pathToFile);

			void Serialize(EntityCLI^ entity);
			EntityCLI^ Deserialize();

			void SerializeEmptyMaterial(System::String^ shaderName, System::String^ materialName);
			Material^ DeserialzeMaterial();
			void Serialize(Material^ material);

			virtual ~GenericSerializerCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!GenericSerializerCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Utils::GenericSerializer* GetHandle()
			{
				return Instance;
			}

		private:
			void SetIsCopy(EntityCLI^ entity);
		};
	};
};