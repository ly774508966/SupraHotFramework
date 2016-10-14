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
			uint32 NameLength = 0;
			char* Name;
			
			uint32 VertexCount = 0;
			uint32 IndexCount = 0;
			uint32 FaceCount = 0;
			uint32 VertexStride = 0;
			uint32 VertexStrideBytes = 0;
			uint32 VertexAttributes = 0;	// VertexBitfield
			uint32 ElementCount = 0;
			uint32 ElementCountBytes = 0;
			uint32 IndexCountBytes = 0;

			uint32 MaterialID = 0;

			float* Vertices;
			uint32* Indices;
		};

		struct Material
		{
			std::string namecopy;

			uint32 NameLength = 0;
			char* Name;

			//uint32 AlbeoMapPathLength = 0;
			std::string AlbedoMapPath;
			
			//uint32 NormalMapPathLength = 0;
			std::string NormalMapPath;

			//uint32 SpecularMapPathLength = 0;
			std::string SpecularMapPath;

			//uint32 ShininessReflectionMapPathLength = 0;
			std::string ShininessReflectionMapPath;

			//uint32 OpacityMapPathLength = 0;
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