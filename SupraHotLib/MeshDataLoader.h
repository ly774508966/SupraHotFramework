#pragma once
#include "Platform.h"
#include "MeshData.h"
#include "MeshComponent.h"
#include <unordered_map>

namespace SupraHot
{
	namespace Utils
	{
		struct MeshLoadData
		{
			std::vector<Graphics::MeshData*> Meshes;
		};

		using namespace Graphics;

		class MeshDataLoader
		{
		private:

			MeshDataLoader();
			MeshLoadData* LoadSHFM(std::string path);
		public:
			static MeshDataLoader* GetInstance();
			~MeshDataLoader();

			//std::vector<MeshComponent*> Load(std::string pathToSHFM);
			std::vector<MeshData*> LoadRawData(std::string pathToSHFM);
			std::vector<MeshDataPtr>* LoadCached(std::string pathToSHFM);
		};
	};
};