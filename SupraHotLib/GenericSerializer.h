#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Utils
	{
		class GenericSerializer
		{
		public:
			GenericSerializer(std::string pathToFile, std::string mode);
			~GenericSerializer();

			void Serialize();
			void Deserialize();
		};
	};
};