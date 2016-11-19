#include "ShaderLibrary.h"


namespace SupraHot
{
	namespace Graphics
	{
		ShaderLibrary::ShaderLibrary()
		{
		}

		ShaderLibrary::~ShaderLibrary()
		{
		}

		void ShaderLibrary::Initialize()
		{
			// Load mesh vertex shaders
			

		}

		void ShaderLibrary::Destroy()
		{
			for (uint32 i = 0, l = VertexShader::StaticMesh::Count; i < l; ++i)
			{
				Shader* shader = MeshStaticVS[i];
				if (shader != nullptr)
				{
					shader->Destroy();
				}
			}

			for (uint32 i = 0, l = PixelShader::Mesh::Count; i < l; ++i)
			{
				Shader* shader = MeshPS[i];
				if (shader != nullptr)
				{
					shader->Destroy();
				}
			}
		}

		ShaderLibrary* ShaderLibrary::GetInstance()
		{
			static ShaderLibrary* instance(new ShaderLibrary);
			return instance;
		}

	};
};