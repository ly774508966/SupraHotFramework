#include "ShaderCompileOptions.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Quat4.h"

namespace SupraHot
{
	namespace Graphics
	{
		ShaderCompileOptions::ShaderCompileOptions()
		{
		}

		ShaderCompileOptions::~ShaderCompileOptions()
		{
		}

		void ShaderCompileOptions::Define(std::string variableName, bool value)
		{
			Definitions.push_back(DefineOption(variableName, std::to_string(value)));
		}

		void ShaderCompileOptions::Define(std::string variableName, uint32 value)
		{
			Definitions.push_back(DefineOption(variableName, std::to_string(value)));
		}

		void ShaderCompileOptions::Define(std::string variableName, float value)
		{
			Definitions.push_back(DefineOption(variableName, std::to_string(value)));
		}

		void ShaderCompileOptions::Define(std::string variableName, Math::Vec2 value)
		{
			std::string valueString = "vec2(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ")";
			Definitions.push_back(DefineOption(variableName, valueString));
		}

		void ShaderCompileOptions::Define(std::string variableName, Math::Vec3 value)
		{
			std::string valueString = "vec3(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ")";
			Definitions.push_back(DefineOption(variableName, valueString));
		}

		void ShaderCompileOptions::Reset()
		{
			Definitions.clear();
		}

		std::vector<ShaderCompileOptions::DefineOption>* ShaderCompileOptions::GetDefinitions()
		{
			return &Definitions;
		}
	};
};