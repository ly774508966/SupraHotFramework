#include "MeshData.h"

namespace SupraHot
{
	namespace Graphics
	{
		MeshData::MeshData()
		{
		}

		MeshData::~MeshData()
		{
		}

		void MeshData::Destroy()
		{
			
			if (HasIndexData)
			{
				glDeleteBuffers(1, &IndexBufferHandle);
			}

			if (HasPositionData || HasNormalData || HasUVData || HasTangentData || HasBiTangentData)
			{
				glDeleteBuffers(1, &VertexBufferHandle);
			}
		}
	};
};