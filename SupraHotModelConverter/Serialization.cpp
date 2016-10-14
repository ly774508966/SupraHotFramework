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
		Write(sizeof(data.Header), 1, &data.Header);
		
		Write(sizeof(data.MeshCount), 1, &data.MeshCount);
		Write(sizeof(Graphics::Mesh), data.MeshCount, &data.Meshes[0]);

		Write(sizeof(data.MaterialCount), 1, &data.MaterialCount);
		Write(sizeof(Graphics::Material), data.MaterialCount, &data.Materials[0]);

		Write(sizeof(data.Footer), 1, &data.Footer);
	}

/*	void Serialization::Write(uint64 size, uint64 amount, const void* data)
	{
		fwrite(data, size, amount, CurrentFile);
	}*/

	template <typename T>
	void Serialization::Write(uint64 size, uint64 amount, const T* data)
	{
		fwrite(data, size, amount, CurrentFile);
	}

	void Serialization::ReadFile(Graphics::SHFModelFile& data)
	{
		Read(sizeof(data.Header), 1, &data.Header);
		
		Read(sizeof(data.MeshCount), 1, &data.MeshCount);

		data.Meshes = new Graphics::Mesh[data.MeshCount];
		Read(sizeof(Graphics::Mesh), data.MeshCount, data.Meshes);

		Read(sizeof(data.MaterialCount), 1, &data.MaterialCount);
		
		data.Materials = new Graphics::Material[data.MaterialCount];
		Read(sizeof(Graphics::Material), data.MaterialCount, data.Materials);

		Read(sizeof(data.Footer), 1, &data.Footer);
	}

/*	void Serialization::Read(uint64 size, uint64 amount, void* data)
	{
		
	}*/

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

			Write(sizeof(mesh.NameLength), 1, &mesh.NameLength);
			Write(sizeof(char), mesh.NameLength, &mesh.Name[0]);
			Write(sizeof(mesh.VertexCount), 1, &mesh.VertexCount);
			Write(sizeof(mesh.IndexCount), 1, &mesh.IndexCount);
			Write(sizeof(mesh.FaceCount), 1, &mesh.FaceCount);
			Write(sizeof(mesh.VertexStride), 1, &mesh.VertexStride);
			Write(sizeof(mesh.VertexStrideBytes), 1, &mesh.VertexStrideBytes);
			Write(sizeof(mesh.VertexAttributes), 1, &mesh.VertexAttributes);
			Write(sizeof(mesh.ElementCount), 1, &mesh.ElementCount);
			Write(sizeof(mesh.ElementCountBytes), 1, &mesh.ElementCountBytes);
			Write(sizeof(mesh.IndexCountBytes), 1, &mesh.IndexCountBytes);
			Write(sizeof(mesh.MaterialID), 1, &mesh.MaterialID);
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

			Write(sizeof(material.NameLength), 1, &material.NameLength);
			Write(sizeof(char), material.NameLength, &material.namecopy.c_str()[0]);
			printf("(Write) material.name | %s \n", material.namecopy.c_str());

			Write(sizeof(material.ID), 1, &material.ID);
			Write(sizeof(material.Ns), 1, &material.Ns);

			Write(sizeof(material.Ka), 1, &material.Ka);
			Write(sizeof(material.Kd), 1, &material.Kd);
			Write(sizeof(material.Ks), 1, &material.Ks);
			Write(sizeof(material.Ke), 1, &material.Ke);

			Write(sizeof(material.Roughness), 1, &material.Roughness);
			Write(sizeof(material.Metalness), 1, &material.Metalness);
			Write(sizeof(material.F0), 1, &material.F0);

			/*
			
			
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
			*/
		}
	}

	void Serialization::Read(uint64 size, uint64 amount, Graphics::Mesh* data)
	{
		
		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Mesh& mesh = data[i];

			Read(sizeof(mesh.NameLength), 1, &mesh.NameLength);

			mesh.Name = new char[mesh.NameLength];
			Read(sizeof(char), mesh.NameLength, mesh.Name);

			Read(sizeof(mesh.VertexCount), 1, &mesh.VertexCount);
			Read(sizeof(mesh.IndexCount), 1, &mesh.IndexCount);
			Read(sizeof(mesh.FaceCount), 1, &mesh.FaceCount);
			Read(sizeof(mesh.VertexStride), 1, &mesh.VertexStride);
			Read(sizeof(mesh.VertexStrideBytes), 1, &mesh.VertexStrideBytes);
			Read(sizeof(mesh.VertexAttributes), 1, &mesh.VertexAttributes);
			Read(sizeof(mesh.ElementCount), 1, &mesh.ElementCount);
			Read(sizeof(mesh.ElementCountBytes), 1, &mesh.ElementCountBytes);
			Read(sizeof(mesh.IndexCountBytes), 1, &mesh.IndexCountBytes);
			Read(sizeof(mesh.MaterialID), 1, &mesh.MaterialID);

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

			Read(sizeof(material.NameLength), 1, &material.NameLength);

			Read(sizeof(char), material.NameLength, (void*)material.namecopy.data());
			material.Name = const_cast<char*>(material.namecopy.c_str());
			
			Read(sizeof(material.ID), 1, &material.ID);
			Read(sizeof(material.Ns), 1, &material.Ns);

			Read(sizeof(material.Ka), 1, &material.Ka);
			Read(sizeof(material.Kd), 1, &material.Kd);
			Read(sizeof(material.Ks), 1, &material.Ks);
			Read(sizeof(material.Ke), 1, &material.Ke);

			Read(sizeof(material.Roughness), 1, &material.Roughness);
			Read(sizeof(material.Metalness), 1, &material.Metalness);
			Read(sizeof(material.F0), 1, &material.F0);

			/*


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
			*/
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
