#include "SkyBox.h"
#include "TextureCube.h"
#include "Camera.h"
#include "Shader.h"

namespace SupraHot
{
	namespace Graphics
	{
		SkyBox::SkyBox()
		{
		}

		SkyBox::~SkyBox()
		{
		}

		void SkyBox::SetEnvironmentMap(TextureCube* enviromentMap)
		{
			this->EnviromentMap = enviromentMap;
		}

		TextureCube* SkyBox::GetEnvironmentMap()
		{
			return EnviromentMap;
		}

		void SkyBox::Render(Camera* camera, Shader* shader)
		{
			// . . . . . . . . . .
		}

		void SkyBox::Destroy()
		{
			if (EnviromentMap != nullptr)
			{
				EnviromentMap->Destroy();
				delete EnviromentMap;
			}
		}

	};
};