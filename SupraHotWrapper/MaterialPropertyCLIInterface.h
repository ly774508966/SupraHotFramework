#pragma once
#include <MaterialProperty.h>

namespace SupraHot
{
	namespace CLI
	{
		template<typename T>
		public interface class MaterialPropertyCLIInterface
		{
		public:
			virtual T GetValue();
			virtual void SetValue(T value);
		};
	};
};