#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "Quat4.h"
#include "Vec2.h"

namespace SupraHot
{
	namespace Graphics
	{
		class Texture2D;
		class Shader;
		class Camera;
		class TextureCube;

		class SkyBox
		{
		private:
			TextureCube* CubeMap = nullptr;
			Texture2D* SphereMap = nullptr;

			// Todo: Use this for rendering
			Math::Quat4 Rotation;
			Math::Vec2 Scale;

			uint32 VAOHandle = 0;
			uint32 IndexVBOHandle = 0;
			uint32 VertexVBOHandle = 0;

			void Render(Shader* shader);
		public:
			SkyBox();
			~SkyBox();

			void SetEnvironmentMap(TextureCube* enviromentMap);
			void SetEnvironmentMap(Texture2D* enviromentMap);
			TextureCube* GetCubeMap();
			Texture2D* GetSphereMap();
			void Render(Camera* camera, Shader* shader);

			void Init();
			void Destroy();
		};
	};
};