#pragma once
#include "Wrapper.h"
#include "MaterialPropertyCLIInterface.h"
#include "Vec2CLI.h"
#include "MaterialPropertyCommonInterface.h"
#include <Vec2MaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class Vec2MaterialPropertyCLI : public MaterialPropertyCLIInterface<Vec2CLI^>, public MaterialPropertyCommonInterface
		{
		protected:
			bool IsCopy = false;
			SupraHot::Graphics::Vec2MaterialProperty* Instance;
		public:

			virtual Vec2CLI^ GetValue()
			{
				Vec2 vec2 = GetHandle()->GetValue();
				return gcnew Vec2CLI(vec2.x, vec2.y);
			};

			virtual void SetValue(Vec2CLI^ value)
			{
				Vec2 vec2(value->x, value->y);
				GetHandle()->SetValue(vec2);
			};

			System::String^ GetType() override;
			System::String^ GetName() override;

			Vec2MaterialPropertyCLI(SupraHot::Graphics::Vec2MaterialProperty*  v2mp);
			Vec2MaterialPropertyCLI(System::String^ name);
			~Vec2MaterialPropertyCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!Vec2MaterialPropertyCLI()
			{
				if (Instance != nullptr  && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Graphics::Vec2MaterialProperty* GetHandle()
			{
				return Instance;
			}

		};
	};
};