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
			std::unordered_map<uint32, Graphics::Material*> MaterialsMap;
			std::vector<uint32> MaterialIDs;
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

			std::vector<MeshComponent*> Load(std::string pathToSHFM);
		};
	};
};