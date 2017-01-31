#include "MaterialCLI.h"
#include "BooleanMaterialPropertyCLI.h"
#include "FloatMaterialPropertyCLI.h"
#include "Vec2MaterialPropertyCLI.h"
#include "Vec3MaterialPropertyCLI.h"
#include "Vec4MaterialPropertyCLI.h"
#include "Texture2DMaterialPropertyCLI.h"
#include "TextureCubeMaterialPropertyCLI.h"
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		Material::Material()
		{
			
		}

		Material::Material(Graphics::MaterialInputs* instance)
		{
			Instance = instance;
		}

		System::String^ Material::GetName()
		{
			std::string& name = Instance->Name;
			return msclr::interop::marshal_as<System::String^>(name);
		}

		System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ Material::GetMaterialProperties()
		{
			System::Collections::Generic::List<MaterialPropertyCommonInterface^>^ mpcis = gcnew System::Collections::Generic::List<MaterialPropertyCommonInterface^>();

			std::vector<SupraHot::Graphics::MaterialProperty*>* materialProperties = Instance->GetMaterialProperties();

			for (size_t i = 0, l = materialProperties->size(); i < l; ++i)
			{
				SupraHot::Graphics::MaterialProperty* materialProperty = materialProperties->at(i);

				SHF_PRINTF("Material type => %s \n", materialProperty->GetType().c_str());

				if (materialProperty->GetType() == "Boolean")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::BooleanMaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::BooleanMaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Float")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::FloatMaterialPropertyCLI
						(
						static_cast<SupraHot::Graphics::FloatMaterialProperty*>(materialProperty)
						)
						);
				}
				else if (materialProperty->GetType() == "Vec2")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec2MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec2MaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Vec3")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec3MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec3MaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "Vec4")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Vec4MaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Vec4MaterialProperty*>(materialProperty)
						)
					);
				} 
				else if (materialProperty->GetType() == "Texture2D")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::Texture2DMaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::Texture2DMaterialProperty*>(materialProperty)
						)
					);
				}
				else if (materialProperty->GetType() == "TextureCube")
				{
					mpcis->Add(
						gcnew SupraHot::CLI::TextureCubeMaterialPropertyCLI
						(
							static_cast<SupraHot::Graphics::TextureCubeMaterialProperty*>(materialProperty)
						)
					);
				}

			}


			return mpcis;
		}

		System::String^ Material::GetShaderDescriptionName()
		{
			return msclr::interop::marshal_as<System::String^>(Instance->GetShaderDescription()->Name);
		}

		void Material::CheckExistingMaterialProperties()
		{
			SHF_PRINTF("Todo: Implement Material::CheckExistingMaterialProperties() \n");
		}

		MaterialPropertyCommonInterface^ Material::AddMaterialProperty(System::String^ name, System::String^ type)
		{
			SHF_PRINTF("Material::AddMaterialProperty (%s | %s) \n", msclr::interop::marshal_as<std::string>(name).c_str(), msclr::interop::marshal_as<std::string>(type).c_str());
			// GetHandle()->AddMaterialProperty(materialProperty->GetHandle());

			std::string materialPropertyName = msclr::interop::marshal_as<std::string>(name);
			// Create a new NATIVE material property based on the incoming data and pass it to a CLI equivilent as an instance
			if (type == "float")
			{
				auto materialProperty = new SupraHot::Graphics::FloatMaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::FloatMaterialPropertyCLI(materialProperty);
			}
			else if (type == "bool")
			{
				auto materialProperty = new SupraHot::Graphics::BooleanMaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::BooleanMaterialPropertyCLI(materialProperty);
			}
			else if (type == "vec2")
			{
				auto materialProperty = new SupraHot::Graphics::Vec2MaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::Vec2MaterialPropertyCLI(materialProperty);
			}
			else if (type == "vec3")
			{
				auto materialProperty = new SupraHot::Graphics::Vec3MaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::Vec3MaterialPropertyCLI(materialProperty);
			}
			else if (type == "vec4")
			{
				auto materialProperty = new SupraHot::Graphics::Vec4MaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::Vec4MaterialPropertyCLI(materialProperty);
			}
			else if (type == "sampler2D")
			{
				// this actually maps "sampler2D" to "Texture2D"
				auto materialProperty = new SupraHot::Graphics::Texture2DMaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::Texture2DMaterialPropertyCLI(materialProperty);
			}
			else if (type == "samplerCube")
			{
				// this actually maps "samplerCube" to "TextureCube"
				auto materialProperty = new SupraHot::Graphics::TextureCubeMaterialProperty(materialPropertyName);

				GetHandle()->AddMaterialProperty(materialProperty);

				return gcnew SupraHot::CLI::TextureCubeMaterialPropertyCLI(materialProperty);
			}

			return nullptr;
		}

		void Material::RemoveMaterialProperty(MaterialPropertyCommonInterface^ materialProperty)
		{
			SHF_PRINTF("Material::RemoveMaterialProperty NEEDS TO BE IMPLEMENTED \n");
		}

		void Material::RemoveMaterialProperty(System::String^ materialName)
		{
			std::string matName = msclr::interop::marshal_as<std::string>(materialName);
			auto materialProperty = GetHandle()->GetMaterialPropertyByName(matName);

			if (materialProperty != nullptr)
			{
				GetHandle()->RemoveMaterialProperty(materialProperty);
			}
		}

		void Material::RemoveAllMaterialProperties()
		{
			GetHandle()->RemoveAllMaterialProperties();
		}

		void Material::SetMaterialFilePath(System::String^ materialPath)
		{
			GetHandle()->SetMaterialFilePath(msclr::interop::marshal_as<std::string>(materialPath));
		}

		System::String^ Material::GetMaterialFilePath()
		{
			return msclr::interop::marshal_as<System::String^>(GetHandle()->GetMaterialFilePath());
		}
	};
};