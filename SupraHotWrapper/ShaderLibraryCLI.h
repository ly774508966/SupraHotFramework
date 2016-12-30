#pragma once
#include "Wrapper.h"
#include <ShaderLibrary.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class ShaderUniformCLI 
		{
		public:
			System::String^ Name;
			System::String^ Type;
		};

		public ref class ShaderLibraryCLI
		{
		private:
			SupraHot::Graphics::ShaderLibrary* Instance;
			static ShaderLibraryCLI^ SingletonInstance;
			ShaderLibraryCLI();

		public:
			static ShaderLibraryCLI^ GetIntance();

			System::Collections::Generic::List<ShaderUniformCLI^>^ GetAvailableShaderUniforms(System::String^ shaderName);
			System::Collections::Generic::List<System::String^>^ GetAvailableShaderNames();

			virtual ~ShaderLibraryCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!ShaderLibraryCLI()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::ShaderLibrary* GetHandle()
			{
				return Instance;
			}
		};
	};
};
