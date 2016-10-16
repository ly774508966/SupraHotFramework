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
			Read(sizeof(uint32), 1, &data.MaterialCount, file);
			data.Materials = new SHFModel::Material[data.MaterialCount];
			Read(sizeof(SHFModel::Material), data.MaterialCount, data.Materials, file);

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
				Read(sizeof(uint32), 1, &mesh.MaterialID, file);

				mesh.Vertices = new float[mesh.ElementCount];
				Read(sizeof(float), mesh.ElementCount, mesh.Vertices, file);

				mesh.Indices = new uint32[mesh.IndexCount];
				Read(sizeof(uint32), mesh.IndexCount, mesh.Indices, file);
			}
		}

		void SHFMBinaryLoader::Read(uint64 size, uint64 amount, SHFModel::Material* data, FILE* file)
		{
			for (uint64 i = 0; i < amount; ++i)
			{
				SHFModel::Material& material = data[i];

				// Read material

				Read(sizeof(uint32), 1, &material.NameLength, file);

				// Note: Yes, yes this sucks... but it works for now.
				// Also, if i just copy the char* directly into the std::string.data(), the app crashes.

				// Read Material name
				void* tempVoidArray = new void*[material.NameLength];
				Read(sizeof(char), material.NameLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.NameLength; c++)
				{
					material.Name += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;

				// Read Albedo map path
				Read(sizeof(uint32), 1, &material.AlbeoMapPathLength, file);
				tempVoidArray = new void*[material.AlbeoMapPathLength];
				Read(sizeof(char), material.AlbeoMapPathLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.AlbeoMapPathLength; c++)
				{
					material.AlbedoMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;


				// Read normal map path
				Read(sizeof(uint32), 1, &material.NormalMapPathLength, file);
				tempVoidArray = new void*[material.NormalMapPathLength];
				Read(sizeof(char), material.NormalMapPathLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.NormalMapPathLength; c++)
				{
					material.NormalMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;

				// Read specular map path
				Read(sizeof(uint32), 1, &material.SpecularMapPathLength, file);
				tempVoidArray = new void*[material.SpecularMapPathLength];
				Read(sizeof(char), material.SpecularMapPathLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.SpecularMapPathLength; c++)
				{
					material.SpecularMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;

				// Read shininess map
				Read(sizeof(uint32), 1, &material.ShininessReflectionMapPathLength, file);
				tempVoidArray = new void*[material.ShininessReflectionMapPathLength];
				Read(sizeof(char), material.ShininessReflectionMapPathLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.ShininessReflectionMapPathLength; c++)
				{
					material.ShininessReflectionMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;

				// Read opacity map path
				Read(sizeof(uint32), 1, &material.OpacityMapPathLength, file);
				tempVoidArray = new void*[material.OpacityMapPathLength];
				Read(sizeof(char), material.OpacityMapPathLength, tempVoidArray, file);
				for (uint32 c = 0; c < material.OpacityMapPathLength; c++)
				{
					material.OpacityMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
				}
				delete tempVoidArray;


				Read(sizeof(uint32), 1, &material.ID, file);
				Read(sizeof(float), 1, &material.Ns, file);

				Read(sizeof(Math::Vec3), 1, &material.Ka, file);
				Read(sizeof(Math::Vec3), 1, &material.Kd, file);
				Read(sizeof(Math::Vec3), 1, &material.Ks, file);
				Read(sizeof(Math::Vec3), 1, &material.Ke, file);

				Read(sizeof(float), 1, &material.Roughness, file);
				Read(sizeof(float), 1, &material.Metalness, file);
				Read(sizeof(float), 1, &material.F0, file);
			}
		}
	};
};