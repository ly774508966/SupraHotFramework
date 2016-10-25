#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Shader;
		class Camera;
		class TextureCube;

		class SkyBox
		{
		private:
			TextureCube* EnviromentMap = nullptr;
		public:
			SkyBox();
			~SkyBox();

			void SetEnvironmentMap(TextureCube* enviromentMap);
			TextureCube* GetEnvironmentMap();
			void Render(Camera* camera, Shader* shader);
			void Destroy();
		};
	};
};