#pragma once

namespace SupraHot
{
	template<typename T>
	public ref class Wrapper
	{
	public:
		bool IsCopy = false;
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
			if (Instance != nullptr && !IsCopy)
			{
				delete Instance;
				Instance = nullptr;
			}
		}

		!Wrapper()
		{
			if (Instance != nullptr && !IsCopy)
			{
				delete Instance;
				Instance = nullptr;
			}
		}

		T* GetHandle()
		{
			return Instance;
		}

		void ReplaceInstance(T* instance)
		{
			if (Instance != nullptr)
			{
				delete Instance;
				Instance = instance;
			}
		}

	};
};