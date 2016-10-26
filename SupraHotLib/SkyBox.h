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

			uint32 VAOHandle = 0;
			uint32 IndexVBOHandle = 0;
			uint32 VertexVBOHandle = 0;

			void Render(Shader* shader);
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