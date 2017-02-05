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

		Write(sizeof(char), 3, &data.Footer);
	}

	template <typename T>
	void Serialization::Write(uint64 size, uint64 amount, const T* data)
	{
		fwrite(data, size, amount, CurrentFile);
	}

	void Serialization::ReadFile(Graphics::SHFModelFile& data)
	{
		data.Header = "";
		void* tempVoidArray = new void*[5];
		Read(sizeof(char), 5, tempVoidArray);
		for (uint32 c = 0; c < 5; c++)
		{
			data.Header += reinterpret_cast<char*>(tempVoidArray)[c];
		}
		delete tempVoidArray;

		Read(sizeof(uint32), 1, &data.MeshCount);

		data.Meshes = new Graphics::Mesh[data.MeshCount];
		Read(sizeof(Graphics::Mesh), data.MeshCount, data.Meshes);

		data.Footer = "";
		tempVoidArray = new void*[3];
		Read(sizeof(char), 3, tempVoidArray);
		for (uint32 c = 0; c < 3; c++)
		{
			data.Footer += reinterpret_cast<char*>(tempVoidArray)[c];
		}
		delete tempVoidArray;
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

			Write(sizeof(float), 1, &mesh.CenterPosition.x);
			Write(sizeof(float), 1, &mesh.CenterPosition.y);
			Write(sizeof(float), 1, &mesh.CenterPosition.z);

			Write(sizeof(float), 1, &mesh.AABBMinimum.x);
			Write(sizeof(float), 1, &mesh.AABBMinimum.y);
			Write(sizeof(float), 1, &mesh.AABBMinimum.z);

			Write(sizeof(float), 1, &mesh.AABBMaximum.x);
			Write(sizeof(float), 1, &mesh.AABBMaximum.y);
			Write(sizeof(float), 1, &mesh.AABBMaximum.z);

			Write(sizeof(float), mesh.ElementCount, &mesh.Vertices[0]);

			Write(sizeof(uint32), mesh.IndexCount, &mesh.Indices[0]);
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

			Read(sizeof(float), 1, &mesh.CenterPosition.x);
			Read(sizeof(float), 1, &mesh.CenterPosition.y);
			Read(sizeof(float), 1, &mesh.CenterPosition.z);

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
