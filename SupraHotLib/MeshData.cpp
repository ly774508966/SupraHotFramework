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
			Destroy();
		}

		void MeshData::Destroy()
		{

#if DEVELOPMENT == 1
			SHF_PRINTF("Destroyed MeshData : %s \n", Name.c_str());
#endif
			if (HasIndexData)
			{
				glDeleteBuffers(1, &IndexBufferHandle);
				HasIndexData = false;
			}

			if (HasPositionData || HasNormalData || HasUVData || HasTangentData || HasBiTangentData)
			{
				glDeleteBuffers(1, &VertexBufferHandle);
				
				HasPositionData = false;
				HasNormalData = false;
				HasUVData = false;
				HasTangentData = false;
				HasBiTangentData = false;
			}
		}
	};
};