// SupraHotModelConverter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Model.h"
#include <iostream>
#include "Serialization.h"

struct Test
{
	char* header[5];

	float x;
	uint32 y;
	float z;

	uint32 dataAmount;
	float* floatData; // this could also be a void* array. then we would have to reintpret_cast<float*> it on read

	char* footer[3];
};

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace SupraHot;
	using namespace Graphics;

	struct Bitf
	{
		unsigned Bit : 1; // -> max size = (2^x - 1)
	};


	std::cout << "Size of float: " << sizeof(float) << std::endl;
	std::cout << "Size of uint32: " << sizeof(uint32) << std::endl;
	std::cout << "Size of uint8 " << sizeof(uint8) << std::endl;

	std::vector<uint8> uint8_Vec;

	Bitf bbbb;
	bbbb.Bit = 2;

	std::cout << bbbb.Bit << std::endl;
	std::cout << "---" << std::endl;

	uint32 attributes = 0;
	
	std::cout << attributes << std::endl;
	
	attributes += POSITION;
	attributes += NORMAL;
	attributes += UV;

	std::cout << attributes << std::endl;

	std::cout << ((POSITION | NORMAL | UV) == attributes) << std::endl;


	Test writeTest = {};
	writeTest.header[0] = "S";
	writeTest.header[1] = "U";
	writeTest.header[2] = "P";
	writeTest.header[3] = "R";
	writeTest.header[4] = "A";
	writeTest.x = 13.37f;
	writeTest.y = 13399;
	writeTest.footer[0] = "H";
	writeTest.footer[1] = "O";
	writeTest.footer[2] = "T";
	writeTest.z = -0.0001345678f;

	std::vector<float> uintVector;
	for (uint32 i = 0; i < 100; i++)
	{
		uintVector.push_back(i);
	}
	uintVector.resize(100);

	writeTest.floatData = uintVector.data();
	writeTest.dataAmount = 100;

	{
		Serialization serialization("writeTest.bin");
		serialization.OpenFile(Serialization::WRITE_BINARY);
		serialization.WriteToFile(writeTest);
		serialization.CloseFile();
	}


	int xxx = 5;

	{
		Test readTest = {};
		printf("Header = %p, footer = %p", readTest.header, readTest.footer);
		printf("x = %f, y = %d, z = %f\n", readTest.x, readTest.y, readTest.z);


		Serialization serialization("writeTest.bin");
		serialization.OpenFile(Serialization::READ_BINARY);
		serialization.ReadFile(readTest);
		serialization.CloseFile();

		std::cout << readTest.footer[2] << std::endl;
		printf("x = %f, y = %d, z = %f\n", readTest.x, readTest.y, readTest.z);
		printf("data amount = %d\n", readTest.dataAmount);

		// reinterpret cast floatData to float*, loop through it and push it to a vector

		//float* floatArr = reinterpret_cast<float*>(readTest.floatData);
		for (uint32 i = 0; i < readTest.dataAmount; i++)
		{
			printf("%f \n", readTest.floatData[i]);
		}

	}

	return 0;
}

