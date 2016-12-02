#pragma once

namespace SupraHot
{
	namespace CLI
	{
		public ref class MaterialPropertyCommonInterface
		{
		public:
			virtual System::String^ GetType() { return "NONE"; };
			virtual System::String^ GetName() { return "NONE"; };
		};
	};
};