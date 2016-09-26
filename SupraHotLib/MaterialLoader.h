#pragma once
#include "Platform.h"
#include "Material.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Utils
	{
		class MaterialLoader
		{
		private:
			MaterialLoader();
		public:
			static MaterialLoader* GetInstance();
			~MaterialLoader();

			// Loads a .mtl file
			std::unordered_map<std::string, Graphics::Material> LoadMTLFile(std::string path);
		};
	};
};

