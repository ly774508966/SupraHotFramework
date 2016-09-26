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

			std::vector<MeshData*> LoadOBJ(std::string path);
			std::vector<MeshData*> LoadOBJX(std::string path);
			std::vector<MeshData*> LoadFBX(std::string path);
			std::vector<MeshData*> LoadWithAssimp(std::string path);
			std::vector<MeshData*> LoadSHMD(std::string path);
		public:
			static MeshDataLoader* GetInstance();
			~MeshDataLoader();

			// Model formats
			enum ModelFileFormat
			{
				OBJ = 0,
				OBJX,
				FBX,
				SHMD	// SupraHotMeshData
			};

			std::vector<MeshData*> Load(std::string path, ModelFileFormat modelFileFormat);
		};
	};
};