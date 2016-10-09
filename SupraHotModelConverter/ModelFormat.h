#include "Utils.h"
#include "Vectors.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		struct Vertex
		{
			Math::Vec3 Position;
			Math::Vec3 Normal;
			Math::Vec2 UV;
			Math::Vec3 Tangent;
			Math::Vec3 BiNormal;
		};

		enum VertexBitfield
		{
			POSITION = BITFIELD(0),
			NORMAL = BITFIELD(1),
			UV = BITFIELD(2),
			TANGENT = BITFIELD(3),
			BINORMAL = BITFIELD(4),
		};

		struct Mesh
		{
			uint32 VertexCount;
			uint32 IndexCount;
			uint32 VertexStride;
			uint32 VertexAttributes;	// VertexBitfield
			std::vector<uint8> Vertices;
			std::vector<uint8> Indices;
		};

		struct SHFModelFile
		{
			char* header = "SPR";

			uint32 MeshCount;
			Mesh* Meshes;

			char* footer = "HOT";
		};
	};
};