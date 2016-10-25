#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "Quat4.h"

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
			Math::Vec3 Position;
			Math::Quat4 Rotation;

			void Render();
		public:
			SkyBox();
			~SkyBox();

			void SetEnvironmentMap(TextureCube* enviromentMap);
			TextureCube* GetEnvironmentMap();
			void Render(Camera* camera, Shader* shader);

			void Init();
			void Destroy();
		};
	};
};