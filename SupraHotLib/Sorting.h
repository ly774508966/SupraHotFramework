#pragma once
#include "Platform.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		class Camera;
	}

	class MeshComponent;

	namespace Utils
	{
		class Sorting
		{
		public:
			static void BubbleSortZ(std::vector<MeshComponent*>& meshcomponentVector, Graphics::Camera* camera);
			static void MergeSort(std::vector<MeshComponent*>& meshcomponentVector, Graphics::Camera* camera);
			static void BubbleSortShaderIndex(std::vector<MeshComponent*>& meshcomponentVector);
			
			// Find Shader indices
			static int FindMaxShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID);
			static int FindMinShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID);
			static int FindShaderInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID);

			static int FindMaxShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID);
			static int FindMinShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID);
			static int FindShaderInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID);
			

			static int FindMaxMaterialIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID);
			static int FindMinMaterialIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID);
			static int FindMaterialInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID);
		};
	};
};