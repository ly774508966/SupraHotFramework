#include "SHFMBinaryLoader.h"
#include "FileSystem.h"

namespace SupraHot
{
	namespace Utils
	{
		SHFMBinaryLoader& SHFMBinaryLoader::GetInstance()
		{
			static SHFMBinaryLoader* instance(new SHFMBinaryLoader);
			return *instance;
		}

		SHFModelFile SHFMBinaryLoader::LoadFromFile(std::string path)
		{
			// ------- Load file

			FILE* file = SupraHot::Utils::FileSystem::GetInstance()->GetFile("", path, "rb");


			SHFModelFile data;
			data.Header = "";
			void* tempVoidArray = new void*[5];
			Read(sizeof(char), 5, tempVoidArray, file);
			for (uint32 c = 0; c < 5; c++)
			{
				data.Header += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;
			
			Read(sizeof(uint32), 1, &data.MeshCount, file);
			data.Meshes = new SHFModel::Mesh[data.MeshCount];
			Read(sizeof(SHFModel::Mesh), data.MeshCount, data.Meshes, file);

			data.Footer = "";
			tempVoidArray = new void*[3];
			Read(sizeof(char), 3, tempVoidArray, file);
			for (uint32 c = 0; c < 3; c++)
			{
				data.Footer += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;
			
			fclose(file);
			return data;
		}

		SHFMBinaryLoader::SHFMBinaryLoader()
		{
		}

		SHFMBinaryLoader::~SHFMBinaryLoader()
		{
		}

		template <typename T>
		void SHFMBinaryLoader::Read(uint64 size, uint64 amount, T* data, FILE* file)
		{
			fread(data, size, amount, file);
		}

		void SHFMBinaryLoader::Read(uint64 size, uint64 amount, SHFModel::Mesh* data, FILE* file)
		{
			for (uint64 i = 0; i < amount; ++i)
			{
				SHFModel::Mesh& mesh = data[i];

				Read(sizeof(uint32), 1, &mesh.NameLength, file);
				void* tempVoidArray = new void*[mesh.NameLength];
				Read(sizeof(char), mesh.NameLength, tempVoidArray, file);
				for (uint32 c = 0; c < mesh.NameLength; c++)
				{
					mesh.Name += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;

				Read(sizeof(uint32), 1, &mesh.VertexCount, file);
				Read(sizeof(uint32), 1, &mesh.IndexCount, file);
				Read(sizeof(uint32), 1, &mesh.FaceCount, file);
				Read(sizeof(uint32), 1, &mesh.VertexStride, file);
				Read(sizeof(uint32), 1, &mesh.VertexStrideBytes, file);
				Read(sizeof(uint32), 1, &mesh.VertexAttributes, file);
				Read(sizeof(uint32), 1, &mesh.ElementCount, file);
				Read(sizeof(uint32), 1, &mesh.ElementCountBytes, file);
				Read(sizeof(uint32), 1, &mesh.IndexCountBytes, file);

				Read(sizeof(float), 1, &mesh.CenterPosition.x, file);
				Read(sizeof(float), 1, &mesh.CenterPosition.y, file);
				Read(sizeof(float), 1, &mesh.CenterPosition.z, file);

				Read(sizeof(float), 1, &mesh.AABBMinimum.x, file);
				Read(sizeof(float), 1, &mesh.AABBMinimum.y, file);
				Read(sizeof(float), 1, &mesh.AABBMinimum.z, file);

				Read(sizeof(float), 1, &mesh.AABBMaximum.x, file);
				Read(sizeof(float), 1, &mesh.AABBMaximum.y, file);
				Read(sizeof(float), 1, &mesh.AABBMaximum.z, file);

				mesh.Vertices = new float[mesh.ElementCount];
				Read(sizeof(float), mesh.ElementCount, mesh.Vertices, file);

				mesh.Indices = new uint32[mesh.IndexCount];
				Read(sizeof(uint32), mesh.IndexCount, mesh.Indices, file);
			}
		}
	};
};