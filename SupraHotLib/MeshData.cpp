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
			// Clear Vectors
			IndexData.clear();
			PositionData.clear();
			NormalData.clear();
			BiTangentData.clear();
			TangentData.clear();
			UVData.clear();

			if (HasIndexData)
			{
				glDeleteBuffers(1, &IndexBufferHandle);
			}

			if (HasPositionData)
			{
				glDeleteBuffers(1, &PositionBufferHandle);
			}

			if (HasNormalData)
			{
				glDeleteBuffers(1, &BiTangentBufferHandle);
			}

			if (HasBiTangentData)
			{
				glDeleteBuffers(1, &BiTangentBufferHandle);
			}

			if (HasTangentData)
			{
				glDeleteBuffers(1, &TangentBufferHandle);
			}

			if (HasUVData)
			{
				glDeleteBuffers(1, &UVBufferHandle);
			}
		}
	};
};