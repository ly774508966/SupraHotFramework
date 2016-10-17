#pragma once
#include "Platform.h"
#include "MeshData.h"

namespace SupraHot
{
	namespace Utils
	{
		using namespace Graphics;

		class MeshDataLoader
		{
		private:
			MeshDataLoader();
			std::vector<MeshData*> LoadSHFM(std::string path);
			std::vector<MeshData*> LoadOBJX(std::string path);
		public:
			static MeshDataLoader* GetInstance();
			~MeshDataLoader();

			std::vector<MeshData*> Load(std::string pathToSHFM);
		};
	};
};