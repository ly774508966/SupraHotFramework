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
			POSITION	= BITSHIFT(0),
			NORMAL		= BITSHIFT(1),
			UV			= BITSHIFT(2),
			TANGENT		= BITSHIFT(3),
			BITANGENT	= BITSHIFT(4),

			// Combos
			POSITION_NORMAL							= POSITION | NORMAL,
			POSITION_UV								= POSITION | UV,
			POSITION_NORMAL_UV						= POSITION | NORMAL | UV,
			POSITION_NORMAL_UV_TANGENT_BITANGENT	= POSITION | NORMAL | UV | TANGENT | BITANGENT,
		};

		struct Mesh
		{
			std::string Name;
			
			uint32 VertexCount = 0;
			uint32 IndexCount = 0;
			uint32 FaceCount = 0;
			uint32 VertexStride = 0;
			uint32 VertexStrideBytes = 0;
			uint32 VertexAttributes = 0;	// VertexBitfield
			uint64 ElementCount = 0;

			uint32 MaterialID = 0;

			float* Vertices;
			uint32* Indices;
		};

		using namespace Math;
		struct Material
		{
			std::string Name;

			std::string AlbedoMapPath;
			std::string NormalMapPath;
			std::string SpecularMapPath;
			std::string ShininessReflectionMapPath;
			std::string OpacityMapPath;
			
			uint32 ID = 0;
			float Ns = 0;				// specular exponent
			Vec3 Ka;					// ambient
			Vec3 Kd;					// diffuse
			Vec3 Ks;					// specular
			Vec3 Ke;					// emissive
			float Roughness = 0.0f;     // default roughness
			float Metalness = 0.0f;     // defaul metalness
			float F0 = 0.04f;           // Fresnel0 0.0f = base reflectivity
		};

		struct SHFModelFile
		{
			std::string Header = "SUPRA";

			uint32 MeshCount;
			Mesh* Meshes;
			uint32 MaterialCount;
			Material* Materials;

			std::string Footer = "HOT";
		};
	};
};