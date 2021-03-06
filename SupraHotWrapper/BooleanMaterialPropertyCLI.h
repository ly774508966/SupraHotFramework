#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "MaterialPropertyCommonInterface.h"
#include <BooleanMaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class BooleanMaterialPropertyCLI : public MaterialPropertyCLIInterface<bool>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::BooleanMaterialProperty* Instance;
		public:

			virtual bool GetValue() 
			{
				return GetHandle()->GetValue();
			};

			virtual void SetValue(bool value)
			{
				GetHandle()->SetValue(value);
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			BooleanMaterialPropertyCLI(SupraHot::Graphics::BooleanMaterialProperty*);
			BooleanMaterialPropertyCLI(System::String^ name);
			~BooleanMaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!BooleanMaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::BooleanMaterialProperty* GetHandle()
			{
				return Instance;
			}
		};
	};
};