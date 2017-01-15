#include "Utils.h"
#include "Vectors.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		using namespace Math;

		struct Vertex
		{
			Vec3 Position;
			Vec3 Normal;
			Vec2 UV;
			Vec3 Tangent;
			Vec3 BiNormal;
		};

		enum VertexBitfield
		{
			POSITION = BITSHIFT(0),
			NORMAL = BITSHIFT(1),
			UV = BITSHIFT(2),
			TANGENT = BITSHIFT(3),
			BITANGENT = BITSHIFT(4),

			// Combos
			POSITION_NORMAL = POSITION | NORMAL,
			POSITION_UV = POSITION | UV,
			POSITION_NORMAL_UV = POSITION | NORMAL | UV,
			POSITION_NORMAL_UV_TANGENT_BITANGENT = POSITION | NORMAL | UV | TANGENT | BITANGENT,
		};

		struct Mesh
		{
			uint32 NameLength = 0;
			std::string Name;

			uint32 VertexCount = 0;
			uint32 IndexCount = 0;
			uint32 FaceCount = 0;
			uint32 VertexStride = 0;
			uint32 VertexStrideBytes = 0;
			uint32 VertexAttributes = 0;	// VertexBitfield
			uint32 ElementCount = 0;
			uint32 ElementCountBytes = 0;
			uint32 IndexCountBytes = 0;
			Vec3 CenterPosition {};

			float* Vertices;
			uint32* Indices;
		};

		struct SHFModelFile
		{
			std::string Header = "SUPRA";

			uint32 MeshCount;
			Mesh* Meshes;

			std::string Footer = "HOT";
		};
	};
};