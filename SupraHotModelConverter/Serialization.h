#pragma once
#include <iostream>
#include "Utils.h"
#include <vector>

namespace SupraHot
{
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

		template<typename T> void WriteToFile(const T& data);
		void Write(uint64 size, const void* data);


		template<typename T> void ReadFile(T& data);
		void Read(uint64 size, void* data);


		void CloseFile();
	};

	template <typename T>
	void Serialization::WriteToFile(const T& data)
	{
		Write(sizeof(T), &data);
	}

	template <typename T>
	void Serialization::ReadFile(T& data)
	{
		Read(sizeof(T), &data);
	}
};
