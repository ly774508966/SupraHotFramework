#pragma once
#include "Platform.h"
#include "WindowInterface.h"

namespace SupraHot
{
	namespace Graphics
	{
		class WindowAndroid : public WindowInterface
		{
		protected:
			void Setup() override;
		public:
			WindowAndroid();
			~WindowAndroid();

			void Init(uint32 width, uint32 height, std::string title) override;
			void Resize(uint32 width, uint32 height) override;
			bool ShouldClose() override;
			void Update() override;
			void Destroy() override;
			void Clear() override;
		};
	};
};

