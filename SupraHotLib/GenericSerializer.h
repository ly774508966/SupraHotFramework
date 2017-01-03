#pragma once
#include "Platform.h"
#include <json11/json11.hpp>

namespace SupraHot
{
	namespace Graphics{
		class MaterialProperty;
		class ShaderMaterial;
	}

	namespace Utils
	{
		enum MODE
		{
			READ_BINARY = 0,
			READ_PLAIN,
			WRITE_BINARY,
			WRITE_PLAIN
		};


		class GenericSerializer
		{
		private:
			std::string Modes[4];
			std::string PathToFile;
			bool FileOpened = false;
			FILE* File = nullptr;
		public:
			GenericSerializer(std::string pathToFile);
			~GenericSerializer();

			void Serialize();
			void Deserialize();

			void Serialize(Graphics::ShaderMaterial* shaderMaterial);
			void Deserialize(Graphics::ShaderMaterial& shaderMaterial);

		private:
			json11::Json GetValueForMaterialProperty(Graphics::MaterialProperty* materialProperty);

			void OpenFile(MODE mode);
			void CloseFile();
		};
	};
};