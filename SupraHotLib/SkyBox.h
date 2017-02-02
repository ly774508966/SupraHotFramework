#pragma once
#include "Platform.h"
#include "Vec3.h"
#include "Quat4.h"
#include "Vec2.h"
#include "TextureCube.h"
#include "Texture2D.h"

namespace SupraHot
{
	namespace Graphics
	{
		
		class Shader;
		class Camera;
		

		class SkyBox
		{
		private:
			TextureCubePtr CubeMap;
			Texture2DPtr SphereMap;

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

			void SetEnvironmentMap(TextureCubePtr enviromentMap);
			void SetEnvironmentMap(Texture2DPtr enviromentMap);
			TextureCubePtr GetCubeMap();
			Texture2DPtr GetSphereMap();
			void Render(Camera* camera, Shader* shader);

			void Init();
			void Destroy();
		};
	};
};