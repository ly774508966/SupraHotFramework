#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "Vec4CLI.h"
#include "MaterialPropertyCommonInterface.h"
#include <Vec4MaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec4MaterialPropertyCLI : public MaterialPropertyCLIInterface<Vec4CLI^>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::Vec4MaterialProperty* Instance;
		public:

			virtual Vec4CLI^ GetValue()
			{
				Vec4 vec4 = GetHandle()->GetValue();
				return gcnew Vec4CLI(vec4.x, vec4.y, vec4.z, vec4.w);
			};

			virtual void SetValue(Vec4CLI^ value)
			{
				Vec4 vec4(value->x, value->y, value->z, value->w);
				GetHandle()->SetValue(vec4);
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			Vec4MaterialPropertyCLI(SupraHot::Graphics::Vec4MaterialProperty*  v3mp);
			Vec4MaterialPropertyCLI(System::String^ name);
			~Vec4MaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!Vec4MaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::Vec4MaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};