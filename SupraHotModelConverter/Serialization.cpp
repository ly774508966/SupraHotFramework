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

			//printf("(W) Mesh: %s \n", data[i].Name.c_str());

			Write(sizeof(mesh.Name), 1, &mesh.Name);
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

	void Serialization::Read(uint64 size, uint64 amount, Graphics::Mesh* data)
	{
		
		for (uint64 i = 0; i < amount; ++i)
		{
			Graphics::Mesh& mesh = data[i];

			Read(sizeof(mesh.Name), 1, &mesh.Name);
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

	void Serialization::CloseFile()
	{
		if (CurrentFile != nullptr)
		{
			fclose(CurrentFile);
		}
	}
};
