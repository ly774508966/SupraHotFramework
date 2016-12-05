#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "Vec3CLI.h"
#include "MaterialPropertyCommonInterface.h"
#include <Vec3MaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec3MaterialPropertyCLI : public MaterialPropertyCLIInterface<Vec3CLI^>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::Vec3MaterialProperty* Instance;
		public:

			virtual Vec3CLI^ GetValue()
			{
				Vec3 vec3 = GetHandle()->GetValue();
				return gcnew Vec3CLI(vec3.x, vec3.y, vec3.z);
			};

			virtual void SetValue(Vec3CLI^ value)
			{
				Vec3 vec3(value->x, value->y, value->z);
				GetHandle()->SetValue(vec3);
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			Vec3MaterialPropertyCLI(SupraHot::Graphics::Vec3MaterialProperty*  v3mp);
			Vec3MaterialPropertyCLI(System::String^ name);
			~Vec3MaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!Vec3MaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::Vec3MaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};