#pragma once
#include "Platform.h"
#include <string>
#include <vector>

namespace SupraHot
{
	namespace Math{
		class Quat4;
		class Vec3;
		struct Vec2;
	}

	namespace Graphics
	{
		class ShaderCompileOptions
		{
		private:

			struct DefineOption
			{
				std::string Name;
				std::string ValueString;

				DefineOption(std::string name, std::string value) : Name(name), ValueString(value)
				{
					
				}
			};

			std::vector<DefineOption> Definitions;

		public:
			ShaderCompileOptions();
			~ShaderCompileOptions();

			void Define(std::string variableName, bool value);
			void Define(std::string variableName, uint32 value);
			void Define(std::string variableName, float value);
			void Define(std::string variableName, Math::Vec2 value);
			void Define(std::string variableName, Math::Vec3 value);

			void Reset();
			std::vector<DefineOption>* GetDefinitions();
		};
	};
};
