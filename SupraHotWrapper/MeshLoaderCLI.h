#pragma once

#include "Wrapper.h"
#include "EntityCLI.h"
#include <MeshDataLoader.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class MeshLoaderCLI
		{
		private: 
			SupraHot::Utils::MeshDataLoader* Instance;

			MeshLoaderCLI();
			static MeshLoaderCLI^ SingletonInstance;
		public:
			static MeshLoaderCLI^ GetIntance();

			SupraHot::CLI::EntityCLI^ LoadSFHM(System::String^ pathToFile);

			virtual ~MeshLoaderCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!MeshLoaderCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Utils::MeshDataLoader* GetHandle()
			{
				return Instance;
			}
		};
	};
};