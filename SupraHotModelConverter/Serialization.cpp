#include "Serialization.h"
#include "ModelFormat.h"

namespace SupraHot
{
	Serialization::Serialization(const std::string pathToFile)
	{
		this->PathToFile = pathToFile;

		modes[0] = "rb";
		modes[1] = "wb";

	}

	Serialization::~Serialization()
	{
	}

	void Serialization::OpenFile(MODE mode)
	{
		fopen_s(&CurrentFile, PathToFile.c_str(), modes[mode].c_str());
	}

	void Serialization::WriteToFile(const Graphics::SHFModelFile& data)
	{
		Write(sizeof(char), 5, &data.Header);
		
		Write(sizeof(uint32), 1, &data.MeshCount);		
		Write(sizeof(Graphics::Mesh), data.MeshCount, &data.Meshes[0]);

		Write(sizeof(uint32), 1, &data.MaterialCount);
		Write(sizeof(Graphics::Material), data.MaterialCount, &data.Materials[0]);

		Write(sizeof(char), 3, &data.Footer);
	}

	template <typename T>
	void Serialization::Write(uint64 size, uint64 amount, const T* data)
	{
		fwrite(data, size, amount, CurrentFile);
	}

	void Serialization::ReadFile(Graphics::SHFModelFile& data)
	{
		Read(sizeof(char), 5, &data.Header);
		
		Read(sizeof(uint32), 1, &data.MeshCount);

		data.Meshes = new Graphics::Mesh[data.MeshCount];
		Read(sizeof(Graphics::Mesh), data.MeshCount, data.Meshes);

		Read(sizeof(uint32), 1, &data.MaterialCount);
		
		data.Materials = new Graphics::Material[data.MaterialCount];
		Read(sizeof(Graphics::Material), data.MaterialCount, data.Materials);

		Read(sizeof(char), 3, &data.Footer);
	}

	template <typename T>
	void Serialization::Read(uint64 size, uint64 amount, T* data)
	{
		fread(data, size, amount, CurrentFile);
	}

	void Serialization::Write(uint64 size, uint64 amount, Graphics::Mesh* data)
	{

		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Mesh& mesh = data[i];

			Write(sizeof(uint32), 1, &mesh.NameLength);
			Write(sizeof(char), mesh.NameLength, &mesh.Name.c_str()[0]);

			Write(sizeof(uint32), 1, &mesh.VertexCount);

			Write(sizeof(uint32), 1, &mesh.IndexCount);
			
			Write(sizeof(uint32), 1, &mesh.FaceCount);
			
			Write(sizeof(uint32), 1, &mesh.VertexStride);
			
			Write(sizeof(uint32), 1, &mesh.VertexStrideBytes);
			
			Write(sizeof(uint32), 1, &mesh.VertexAttributes);
			
			Write(sizeof(uint32), 1, &mesh.ElementCount);
			
			Write(sizeof(uint32), 1, &mesh.ElementCountBytes);
			
			Write(sizeof(uint32), 1, &mesh.IndexCountBytes);
			
			Write(sizeof(uint32), 1, &mesh.MaterialID);
			
			Write(sizeof(float), mesh.ElementCount, &mesh.Vertices[0]);
			
			Write(sizeof(uint32), mesh.IndexCount, &mesh.Indices[0]);
		}

	}

	void Serialization::Write(uint64 size, uint64 amount, Graphics::Material* data)
	{
		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Material& material = data[i];

			// Write material

			Write(sizeof(uint32), 1, &material.NameLength);
			Write(sizeof(char), material.NameLength, &material.Name.c_str()[0]);

			Write(sizeof(uint32), 1, &material.AlbeoMapPathLength);
			Write(sizeof(char), material.AlbeoMapPathLength, &material.AlbedoMapPath.c_str()[0]);

			Write(sizeof(uint32), 1, &material.NormalMapPathLength);
			Write(sizeof(char), material.NormalMapPathLength, &material.NormalMapPath.c_str()[0]);

			Write(sizeof(uint32), 1, &material.SpecularMapPathLength);
			Write(sizeof(char), material.SpecularMapPathLength, &material.SpecularMapPath.c_str()[0]);

			Write(sizeof(uint32), 1, &material.ShininessReflectionMapPathLength);
			Write(sizeof(char), material.ShininessReflectionMapPathLength, &material.ShininessReflectionMapPath.c_str()[0]);

			Write(sizeof(uint32), 1, &material.OpacityMapPathLength);
			Write(sizeof(char), material.OpacityMapPathLength, &material.OpacityMapPath.c_str()[0]);

			Write(sizeof(uint32), 1, &material.ID);
			Write(sizeof(float), 1, &material.Ns);

			Write(sizeof(Math::Vec3), 1, &material.Ka);
			Write(sizeof(Math::Vec3), 1, &material.Kd);
			Write(sizeof(Math::Vec3), 1, &material.Ks);
			Write(sizeof(Math::Vec3), 1, &material.Ke);

			Write(sizeof(float), 1, &material.Roughness);
			Write(sizeof(float), 1, &material.Metalness);
			Write(sizeof(float), 1, &material.F0);
		}
	}

	void Serialization::Read(uint64 size, uint64 amount, Graphics::Mesh* data)
	{
		
		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Mesh& mesh = data[i];

			Read(sizeof(uint32), 1, &mesh.NameLength);
			void* tempVoidArray = new void*[mesh.NameLength];
			Read(sizeof(char), mesh.NameLength, tempVoidArray);
			for (uint32 c = 0; c < mesh.NameLength; c++)
			{
				mesh.Name += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;

			Read(sizeof(uint32), 1, &mesh.VertexCount);
			Read(sizeof(uint32), 1, &mesh.IndexCount);
			Read(sizeof(uint32), 1, &mesh.FaceCount);
			Read(sizeof(uint32), 1, &mesh.VertexStride);
			Read(sizeof(uint32), 1, &mesh.VertexStrideBytes);
			Read(sizeof(uint32), 1, &mesh.VertexAttributes);
			Read(sizeof(uint32), 1, &mesh.ElementCount);
			Read(sizeof(uint32), 1, &mesh.ElementCountBytes);
			Read(sizeof(uint32), 1, &mesh.IndexCountBytes);
			Read(sizeof(uint32), 1, &mesh.MaterialID);

			mesh.Vertices = new float[mesh.ElementCount];
			Read(sizeof(float), mesh.ElementCount, mesh.Vertices);

			mesh.Indices = new uint32[mesh.IndexCount];
			Read(sizeof(uint32), mesh.IndexCount, mesh.Indices);
		}
	}

	void Serialization::Read(uint64 size, uint64 amount, Graphics::Material* data)
	{
		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Material& material = data[i];

			// Read material

			Read(sizeof(uint32), 1, &material.NameLength);

			// Note: Yes, yes this sucks... but it works for now.
			// Also, if i just copy the char* directly into the std::string.data(), the app crashes.

			// Read Material name
			void* tempVoidArray = new void*[material.NameLength];
			Read(sizeof(char), material.NameLength, tempVoidArray);
			for (uint32 c = 0; c < material.NameLength; c++)
			{
				material.Name += reinterpret_cast<char*>(tempVoidArray)[c];
			}			
			delete tempVoidArray;
			
			// Read Albedo map path
			Read(sizeof(uint32), 1, &material.AlbeoMapPathLength);
			tempVoidArray = new void*[material.AlbeoMapPathLength];
			Read(sizeof(char), material.AlbeoMapPathLength, tempVoidArray);
			for (uint32 c = 0; c < material.AlbeoMapPathLength; c++)
			{
				material.AlbedoMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;


			// Read normal map path
			Read(sizeof(uint32), 1, &material.NormalMapPathLength);
			tempVoidArray = new void*[material.NormalMapPathLength];
			Read(sizeof(char), material.NormalMapPathLength, tempVoidArray);
			for (uint32 c = 0; c < material.NormalMapPathLength; c++)
			{
				material.NormalMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;

			// Read specular map path
			Read(sizeof(uint32), 1, &material.SpecularMapPathLength);
			tempVoidArray = new void*[material.SpecularMapPathLength];
			Read(sizeof(char), material.SpecularMapPathLength, tempVoidArray);
			for (uint32 c = 0; c < material.SpecularMapPathLength; c++)
			{
				material.SpecularMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;

			// Read shininess map
			Read(sizeof(uint32), 1, &material.ShininessReflectionMapPathLength);
			tempVoidArray = new void*[material.ShininessReflectionMapPathLength];
			Read(sizeof(char), material.ShininessReflectionMapPathLength, tempVoidArray);
			for (uint32 c = 0; c < material.ShininessReflectionMapPathLength; c++)
			{
				material.ShininessReflectionMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;

			// Read opacity map path
			Read(sizeof(uint32), 1, &material.OpacityMapPathLength);
			tempVoidArray = new void*[material.OpacityMapPathLength];
			Read(sizeof(char), material.OpacityMapPathLength, tempVoidArray);
			for (uint32 c = 0; c < material.OpacityMapPathLength; c++)
			{
				material.OpacityMapPath += reinterpret_cast<char*>(tempVoidArray)[c];
			}
			delete tempVoidArray;

			
			Read(sizeof(uint32), 1, &material.ID);
			Read(sizeof(float), 1, &material.Ns);

			Read(sizeof(Math::Vec3), 1, &material.Ka);
			Read(sizeof(Math::Vec3), 1, &material.Kd);
			Read(sizeof(Math::Vec3), 1, &material.Ks);
			Read(sizeof(Math::Vec3), 1, &material.Ke);

			Read(sizeof(float), 1, &material.Roughness);
			Read(sizeof(float), 1, &material.Metalness);
			Read(sizeof(float), 1, &material.F0);
		}
	}

	void Serialization::CloseFile()
	{
		if (CurrentFile != nullptr)
		{
			fclose(CurrentFile);
		}
	}
};
