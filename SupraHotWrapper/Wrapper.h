#pragma once

namespace SupraHot
{
	namespace Graphics
	{
		template<typename T>
		public ref class Wrapper
		{
		protected:
			T* Instance;
		public:
			Wrapper()
			{
				Instance = new T();
			}

			Wrapper(T* instance)
				: Instance(instance)
			{
			}

			virtual ~Wrapper()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			!Wrapper()
			{
				if (Instance != nullptr)
				{
					delete Instance;
					Instance = nullptr;
				}
			}

			T* GetHandle()
			{
				return Instance;
			}

		};

	};
};