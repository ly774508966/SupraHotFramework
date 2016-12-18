#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "MaterialPropertyCommonInterface.h"
#include <FloatMaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class FloatMaterialPropertyCLI : public MaterialPropertyCLIInterface<float>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::FloatMaterialProperty* Instance;
		public:

			virtual float GetValue()
			{
				return GetHandle()->GetValue();
			};

			virtual void SetValue(float value)
			{
				GetHandle()->SetValue(value);
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			FloatMaterialPropertyCLI(SupraHot::Graphics::FloatMaterialProperty*  fmp);
			FloatMaterialPropertyCLI(System::String^ name);
			~FloatMaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!FloatMaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::FloatMaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};