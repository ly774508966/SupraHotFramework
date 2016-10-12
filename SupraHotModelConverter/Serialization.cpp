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
		printf("WRITE MESH \n");
		fwrite(data, size, amount, CurrentFile);
	}

	void Serialization::Read(uint64 size, uint64 amount, Graphics::Mesh* data)
	{
		printf("READ MESH \n");
		fread(data, size, amount, CurrentFile);
	}

	void Serialization::CloseFile()
	{
		if (CurrentFile != nullptr)
		{
			fclose(CurrentFile);
		}
	}
};
