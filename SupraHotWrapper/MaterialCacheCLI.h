#pragma once
#include <Platform.h>
#include <MaterialCache.h>
#include "MaterialCLI.h"

namespace SupraHot
{
	namespace CLI
	{
		public ref class MaterialCacheCLI
		{
		private:
			SupraHot::Graphics::MaterialCache* Instance;
			static MaterialCacheCLI^ SingletonInstance;
			MaterialCacheCLI();
		public:
			static MaterialCacheCLI^ GetIntance();
			~MaterialCacheCLI();

			void LoadIntoCache(System::String^ pathToMaterialFile);
			Material^ GetCached(System::String^ pathToMaterialFile);
			bool IsCached(System::String^ pathToMaterialFile);
		};

	};
};