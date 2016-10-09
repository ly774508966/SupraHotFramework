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
			POSITION	= BITFIELD(0),
			NORMAL		= BITFIELD(1),
			UV			= BITFIELD(2),
			TANGENT		= BITFIELD(3),
			BITANGENT	= BITFIELD(4),

			// Combos
			POSITION_NORMAL							= POSITION | NORMAL,
			POSITION_UV								= POSITION | UV,
			POSITION_NORMAL_UV						= POSITION | NORMAL | UV,
			POSITION_NORMAL_UV_TANGENT_BITANGENT	= POSITION | NORMAL | UV | TANGENT | BITANGENT,
		};

		struct Mesh
		{
			char* Name;
			uint32 VertexCount;
			uint32 IndexCount;
			uint32 VertexStride;
			uint32 VertexAttributes;	// VertexBitfield
			float* Vertices;
			uint32* Indices;
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