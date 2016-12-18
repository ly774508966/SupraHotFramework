#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "MaterialPropertyCommonInterface.h"
#include <TextureCubeMaterialProperty.h>
#include <msclr\marshal_cppstd.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class TextureCubeMaterialPropertyCLI : public MaterialPropertyCLIInterface<System::String^>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::TextureCubeMaterialProperty* Instance;
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

			TextureCubeMaterialPropertyCLI(SupraHot::Graphics::TextureCubeMaterialProperty*  v3mp);
			TextureCubeMaterialPropertyCLI(System::String^ name);
			~TextureCubeMaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!TextureCubeMaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::TextureCubeMaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};