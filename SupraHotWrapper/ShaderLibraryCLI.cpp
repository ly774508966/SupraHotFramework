#include "ShaderLibraryCLI.h"
#include <ShaderDescription.h>
#include <ShaderParser.h>
#include <FileSystem.h>
#include <unordered_map>
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{

	namespace CLI
	{
		ShaderLibraryCLI::ShaderLibraryCLI()
		{
			Instance = SupraHot::Graphics::ShaderLibrary::GetInstance();
		}

		ShaderLibraryCLI^ ShaderLibraryCLI::GetIntance()
		{
			if (SingletonInstance == nullptr)
			{
				SingletonInstance = gcnew ShaderLibraryCLI();
			}

			return SingletonInstance;
		}

		System::Collections::Generic::List<System::String^>^ ShaderLibraryCLI::GetAvailableShaderNames()
		{
			std::unordered_map<std::string, SupraHot::Graphics::ShaderDescription*>* ShaderDescriptions = GetHandle()->GetShaderDescriptions();
			System::Collections::Generic::List<System::String^>^ shaderNames = gcnew System::Collections::Generic::List<System::String^>();

			typedef std::unordered_map<std::string, SupraHot::Graphics::ShaderDescription*>::iterator it;

			for (it iterator = ShaderDescriptions->begin(); iterator != ShaderDescriptions->end(); ++iterator)
			{

				shaderNames->Add(msclr::interop::marshal_as<System::String^>(iterator->first));
			}

			return shaderNames;
		}


		System::Collections::Generic::List<ShaderUniformCLI^>^ ShaderLibraryCLI::GetAvailableShaderUniforms(System::String^ shaderName)
		{
			std::unordered_map<std::string, SupraHot::Graphics::ShaderDescription*>* ShaderDescriptions = GetHandle()->GetShaderDescriptions();
			System::Collections::Generic::List<ShaderUniformCLI^>^ shaderUniforms = gcnew System::Collections::Generic::List<ShaderUniformCLI^>();

			std::string shaderNameStd = msclr::interop::marshal_as<std::string>(shaderName);

			// GetShaderDescriptions()->find()
			if (ShaderDescriptions->find(shaderNameStd) != ShaderDescriptions->end())
			{
				SHF_PRINTF("Found shader description for %s \n", shaderNameStd.c_str());

				SupraHot::Graphics::ShaderDescription* shaderDescription = ShaderDescriptions->at(shaderNameStd);
				std::unordered_map<std::string, std::string>* uniforms = &shaderDescription->Uniforms;

				typedef std::unordered_map<std::string, std::string>::iterator it;
				for (it iterator = shaderDescription->Uniforms.begin(); iterator != shaderDescription->Uniforms.end(); ++iterator)
				{
					std::string name = iterator->first;
					std::string type = iterator->second;

					ShaderUniformCLI^ shaderUniform = gcnew ShaderUniformCLI();
					shaderUniform->Name = msclr::interop::marshal_as<System::String^>(name);
					shaderUniform->Type = msclr::interop::marshal_as<System::String^>(type);
					shaderUniforms->Add(shaderUniform);
				}
			}

			return shaderUniforms;
		}

		void ShaderLibraryCLI::LoadShaderIfNotAlreadyLoaded(System::String^ shaderFilePath)
		{
			std::string shaderfilePathStd = "Shaders/Description/" + msclr::interop::marshal_as<std::string>(shaderFilePath);

			// Open json and read the shader name from the header.
			// Check if shader exists already
			if (SupraHot::Utils::FileSystem::GetInstance()->FileExists("", shaderfilePathStd))
			{
				SupraHot::Graphics::ShaderDescription* shaderDescription = SupraHot::Graphics::ShaderParser::GetInstance()->Parse(shaderfilePathStd);
				
				std::string shaderName = shaderDescription->Name;

				std::unordered_map<std::string, Graphics::ShaderDescription*>* shaderDescriptions = Graphics::ShaderLibrary::GetInstance()->GetShaderDescriptions();
				if (shaderDescriptions->find(shaderName) == shaderDescriptions->end())
				{
					//  Save
					Graphics::ShaderLibrary::GetInstance()->ProcessShaderDescription(shaderDescription);
				}
				else
				{
					// Delete description, if we already have the shader loaded 
					delete shaderDescription;
				}
			}

		}

	};
};
