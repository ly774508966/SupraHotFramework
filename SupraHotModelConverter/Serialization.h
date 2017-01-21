#pragma once
#include <iostream>
#include "Utils.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		struct Mesh;
		struct SHFModelFile;
	}

	class Serialization
	{
	private:
		FILE* CurrentFile;
		std::string PathToFile;

		std::string modes[2];

	public:
		enum MODE
		{
			READ_BINARY = 0,
			WRITE_BINARY
		};

		Serialization(std::string pathToFile);
		~Serialization();

		void OpenFile(MODE mode);

		void WriteToFile(const Graphics::SHFModelFile& data);
		//void Write(uint64 size, uint64 amount, const void* data);
		template<typename T> void Write(uint64 size, uint64 amount, const T* data);
		void Write(uint64 size, uint64 amount, Graphics::Mesh* data);

		void ReadFile(Graphics::SHFModelFile& data);
		//void Read(uint64 size, uint64 amount, void* data);
		template<typename T> void Read(uint64 size, uint64 amount, T* data);
		void Read(uint64 size, uint64 amount, Graphics::Mesh* data);

		void CloseFile();
	};
};
