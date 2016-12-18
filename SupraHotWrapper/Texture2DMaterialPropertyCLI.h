#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "MaterialPropertyCommonInterface.h"
#include <Texture2DMaterialProperty.h>
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Texture2DMaterialPropertyCLI : public MaterialPropertyCLIInterface<System::String^>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::Texture2DMaterialProperty* Instance;
		public:

			virtual System::String^ GetValue()
			{
				const std::string& type = GetHandle()->GetValue();
				return msclr::interop::marshal_as<System::String^>(type);
			};

			virtual void SetValue(System::String^ value)
			{
				GetHandle()->SetValue(msclr::interop::marshal_as<std::string>(value));
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			Texture2DMaterialPropertyCLI(SupraHot::Graphics::Texture2DMaterialProperty*  v3mp);
			Texture2DMaterialPropertyCLI(System::String^ name);
			~Texture2DMaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!Texture2DMaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::Texture2DMaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};