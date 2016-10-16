#pragma once
#include <string>
#include "Vec3.h"
#include "Platform.h"

namespace SupraHot
{
	namespace Utils
	{
		namespace SHFModel
		{	
			#define BITSHIFT(num) (1 << num)
			using namespace Math;

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

				uint32 MaterialID = 0;

				float* Vertices;
				uint32* Indices;
			};

			struct Material
			{
				uint32 NameLength = 0;
				std::string Name;

				uint32 AlbeoMapPathLength = 0;
				std::string AlbedoMapPath;

				uint32 NormalMapPathLength = 0;
				std::string NormalMapPath;

				uint32 SpecularMapPathLength = 0;
				std::string SpecularMapPath;

				uint32 ShininessReflectionMapPathLength = 0;
				std::string ShininessReflectionMapPath;

				uint32 OpacityMapPathLength = 0;
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
		};

		struct SHFModelFile
		{
			std::string Header = "SUPRA";

			uint32 MeshCount;
			SHFModel::Mesh* Meshes;

			uint32 MaterialCount;
			SHFModel::Material* Materials;

			std::string Footer = "HOT";
		};

		class SHFMBinaryLoader
		{
		private:
			SHFMBinaryLoader();
			template<typename T> void Read(uint64 size, uint64 amount, T* data, FILE* file);
			void Read(uint64 size, uint64 amount, SHFModel::Mesh* data, FILE* file);
			void Read(uint64 size, uint64 amount, SHFModel::Material* data, FILE* file);
		public:
			static SHFMBinaryLoader& GetInstance();
			SHFModelFile LoadFromFile(std::string path);
			~SHFMBinaryLoader();
		};
	};
};