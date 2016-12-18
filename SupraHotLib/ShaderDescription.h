#pragma once
#include "Platform.h"
#include <unordered_map>
#include <vector>
#include "ShaderLibrary.h"

namespace SupraHot
{
	namespace Graphics
	{
		class ShaderDescription
		{
		public:
			// This gets generated by the ShaderParser
			std::string Name = "";
			std::string Description = "";
			std::string VertexShaderPath = "";
			std::string PixelShaderPath = "";
			ShaderLibrary::BRDFType BRDFType = ShaderLibrary::BRDFType::None;
			std::unordered_map<std::string, std::vector<std::string>> DefinedWhen;
			std::unordered_map<std::string, std::vector<std::string>> Dependencies;
			std::unordered_map<std::string, std::string> Uniforms;

			// This gets generated by the ShaderLibrary
			std::unordered_map<std::string, uint64> BitShiftedIndices;
			ShaderDescription();
			~ShaderDescription();
		};
	};
};
