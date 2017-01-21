#include "Mesh.h"

namespace SupraHot
{
	namespace Graphics
	{
		Mesh::Mesh()
		{
		}

		Mesh::Mesh(MeshData* meshData)
		{
			this->Load(meshData);
		}

		Mesh::~Mesh()
		{
		}

		void Mesh::Load(MeshData* meshData)
		{
		}

		const uint32& Mesh::GetVertexHandle()
		{
			return VertexHandle;
		}

		const uint32& Mesh::GetUVHandle()
		{
			return UVHandle;
		}

		const uint32& Mesh::GetNormalHandle()
		{
			return NormalHandle;
		}

		const uint32& Mesh::GetTangentHandle()
		{
			return TangentHandle;
		}

		const uint32& Mesh::GetBiNormalHandle()
		{
			return BiNormalHandle;
		}

		void Mesh::Destroy()
		{
		}
	};
};
