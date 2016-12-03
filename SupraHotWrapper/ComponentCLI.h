#pragma once
#include "Wrapper.h"
#include <Component.h>

namespace SupraHot
{
	namespace CLI
	{
		public ref class ComponentCLI
		{
		public:
			bool IsCopy = false;
		protected:
			SupraHot::Component* Instance;
		public:

			ComponentCLI()
			{
			}

			ComponentCLI(SupraHot::Component* instance) : Instance(instance)
			{
			}

			generic <class T>
				where T : ComponentCLI, gcnew()
			T As()
			{
				System::Type^ type = T::typeid;

				T converted = gcnew T();
				converted->IsCopy = true;
				converted->ReplaceInstance(Instance);
				return converted;

			}

			~ComponentCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!ComponentCLI()
			{
				if (Instance != nullptr && !IsCopy)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			SupraHot::Component* GetHandle()
			{
				return Instance;
			}

			void ReplaceInstance(SupraHot::Component* instance)
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = instance;
				}
			}

			void Register(Entity* parent)
			{
				Instance->Register(parent);
			};

			void Remove()
			{
				Instance->Remove();
			};

			virtual void Registered()
			{
				Instance->Registered();
			};

			virtual void Removed()
			{
				Instance->Removed();
			};

			virtual void Destroy()
			{
				Instance->Destroy();
			};

			virtual void Update(float deltaTime)
			{
				Instance->Update(deltaTime);
			};

			virtual void LateUpdate(float deltaTime)
			{
				Instance->LateUpdate(deltaTime);
			};

			virtual void FixedUpdate(float deltaTime)
			{
				Instance->FixedUpdate(deltaTime);
			};


		};
	};
};