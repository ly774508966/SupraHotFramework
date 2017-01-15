#pragma once
#include "Platform.h"
#include "Entity.h"
#include <json11/json11.hpp>

namespace SupraHot
{
	class MeshComponent;

	namespace Graphics 
	{
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

			void Serialize(Entity* Entity);
			void Deserialize(Entity* Entity);

			void Serialize(MeshComponent* meshComponent);
			void Deserialize(MeshComponent& meshComponent);

		private:
			json11::Json GetValueForMaterialProperty(Graphics::MaterialProperty* materialProperty);
			json11::Json GetValueForTransform(Transform& transform);
			json11::Json SerializeChild(Entity* entity);
			json11::Json SerializeComponent(Component* component);
			json11::Json SerializeShaderMaterial(Graphics::ShaderMaterial* shaderMaterial);

			void OpenFile(MODE mode);
			void CloseFile();
		};
	};
};