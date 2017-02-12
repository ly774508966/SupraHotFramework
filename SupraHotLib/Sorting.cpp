#include "Sorting.h"
#include "Platform.h"
#include "Vec3.h"
#include "Camera.h"
#include "MeshComponent.h"
#include "Entity.h"
#include <cmath>

namespace SupraHot
{
	namespace Utils
	{
		void Sorting::BubbleSortZ(std::vector<MeshComponent*>& meshcomponentVector, Graphics::Camera* camera)
		{
			Vec3 cameraPosition = camera->Position;

			bool swapped = false;
			uint32 n = static_cast<uint32>(meshcomponentVector.size());
			uint32 rightMarker = n;

			do 
			{
				if (swapped) 
				{
					n = rightMarker;
				}
				swapped = false;
				for (uint32 i = 0; i < n - 1; i++)
				{
					float distanceToCameraSqrLeft = (meshcomponentVector.at(i)->GetParent()->GetTransform().GetGlobalPosition() - cameraPosition).lengthSqr();
					float distanceToCameraSqrRight = (meshcomponentVector.at(i + 1)->GetParent()->GetTransform().GetGlobalPosition() - cameraPosition).lengthSqr();

					if (distanceToCameraSqrLeft < distanceToCameraSqrRight)
					{
						MeshComponent* swap = meshcomponentVector[i];
						meshcomponentVector[i] = meshcomponentVector[i + 1];
						meshcomponentVector[i + 1] = swap;
						rightMarker = i + 1;
						swapped = true;
					}
				}
			} 
			while (swapped);

		}

		void Sorting::MergeSort(std::vector<MeshComponent*>& meshcomponentVector, Graphics::Camera* camera)
		{

		}

		void Sorting::BubbleSortShaderIndex(std::vector<MeshComponent*>& meshcomponentVector)
		{
			bool swapped = false;
			uint32 n = static_cast<uint32>(meshcomponentVector.size());
			uint32 rightMarker = n;

			do
			{
				if (swapped)
				{
					n = rightMarker;
				}
				swapped = false;
				for (uint32 i = 0; i < n - 1; i++)
				{
					uint64 shaderIndexLeft = meshcomponentVector.at(i)->GetMaterial()->GetShader()->GetShaderPermutationIndex();
					uint64 shaderIndexRight = meshcomponentVector.at(i + 1)->GetMaterial()->GetShader()->GetShaderPermutationIndex();

					if (shaderIndexLeft < shaderIndexRight)
					{
						MeshComponent* swap = meshcomponentVector[i];
						meshcomponentVector[i] = meshcomponentVector[i + 1];
						meshcomponentVector[i + 1] = swap;
						rightMarker = i + 1;
						swapped = true;
					}
				}
			} while (swapped);
		}

		int Sorting::FindMaxShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID)
		{
			return FindMaxShaderIndex(meshcomponentVector, 0, static_cast<int>(meshcomponentVector.size() - 1), shaderUUID);
		}

		int Sorting::FindMinShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID)
		{
			return FindMinShaderIndex(meshcomponentVector, 0, static_cast<int>(meshcomponentVector.size() - 1), shaderUUID);
		}

		int Sorting::FindMaxShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID)
		{
			int centerPivot = leftPivot + floor((rightPivot - leftPivot) / 2);
			uint64 elementAtCenter = meshcomponentVector[centerPivot]->GetMaterial()->GetShader()->GetUUID();
			int maxIndex = -1;

			if (leftPivot > rightPivot)
			{
				return -1;
			}

			if ((rightPivot - leftPivot) == 2
				|| centerPivot == leftPivot
				|| centerPivot == rightPivot)
			{
				if (meshcomponentVector[leftPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID) 
				{
					maxIndex = leftPivot;
				}

				if (meshcomponentVector[centerPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID) 
				{
					maxIndex = centerPivot;
				}

				if (meshcomponentVector[rightPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID) 
				{
					maxIndex = rightPivot;
				}

				return maxIndex;
			}
		
			if (elementAtCenter <= shaderUUID)
			{
				return FindMaxShaderIndex(meshcomponentVector, centerPivot, rightPivot, shaderUUID);
			}
			else if (elementAtCenter > shaderUUID)
			{
				return FindMaxShaderIndex(meshcomponentVector, leftPivot, centerPivot, shaderUUID);
			}

			return -1;
		}

		int Sorting::FindMinShaderIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID)
		{
			int centerPivot = leftPivot + floor((rightPivot - leftPivot) / 2);
			uint64 elementAtCenter = meshcomponentVector[centerPivot]->GetMaterial()->GetShader()->GetUUID();
			int maxIndex = -1;

			if (leftPivot > rightPivot)
			{
				return -1;
			}

			if ((rightPivot - leftPivot) == 2
				|| centerPivot == leftPivot
				|| centerPivot == rightPivot)
			{
				

				if (meshcomponentVector[rightPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID) 
				{
					maxIndex = rightPivot;
				}

				if (meshcomponentVector[centerPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID)
				{
					maxIndex = centerPivot;
				}

				if (meshcomponentVector[leftPivot]->GetMaterial()->GetShader()->GetUUID() == shaderUUID) 
				{
					maxIndex = leftPivot;
				}

				return maxIndex;
			}

			if (elementAtCenter < shaderUUID) 
			{
				return FindMinShaderIndex(meshcomponentVector, centerPivot, rightPivot, shaderUUID);
			}
			else if (elementAtCenter >= shaderUUID) 
			{
				return FindMinShaderIndex(meshcomponentVector, leftPivot, centerPivot, shaderUUID);
			}

			return -1;
		}

		int Sorting::FindShaderInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 shaderUUID)
		{			
			// Here we need to find an index at which 
			// the left element is smaller and the right element is bigger than the given shader uuid.

			// Todo: improve this!
			// We do not want to do a linear search.
			for (int i = leftPivot; i < rightPivot; ++i) {
				if (meshcomponentVector[i]->GetMaterial()->GetShader()->GetUUID() > shaderUUID) {
					return i;
				}
			}

			return rightPivot;
		}

		int Sorting::FindShaderInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, uint64 shaderUUID)
		{
			return FindShaderInsertionIndex(meshcomponentVector, 0, static_cast<int>(meshcomponentVector.size()), shaderUUID);
		}

		int Sorting::FindMaxMaterialIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID)
		{
			int centerPivot = leftPivot + floor((rightPivot - leftPivot) / 2);
			uint64 elementAtCenter = meshcomponentVector[centerPivot]->GetMaterial()->GetUUID();
			int maxIndex = -1;

			if (leftPivot > rightPivot)
			{
				return -1;
			}

			if ((rightPivot - leftPivot) == 2
				|| centerPivot == leftPivot
				|| centerPivot == rightPivot)
			{
				if (meshcomponentVector[leftPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = leftPivot;
				}

				if (meshcomponentVector[centerPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = centerPivot;
				}

				if (meshcomponentVector[rightPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = rightPivot;
				}

				return maxIndex;
			}

			if (elementAtCenter <= materialUUID)
			{

				return FindMaxMaterialIndex(meshcomponentVector, centerPivot, rightPivot, materialUUID);
			}
			else if (elementAtCenter > materialUUID)
			{
				return FindMaxMaterialIndex(meshcomponentVector, leftPivot, centerPivot, materialUUID);
			}

			return -1;
		}

		int Sorting::FindMinMaterialIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID)
		{
			int centerPivot = leftPivot + floor((rightPivot - leftPivot) / 2);
			uint64 elementAtCenter = meshcomponentVector[centerPivot]->GetMaterial()->GetUUID();
			int maxIndex = -1;

			if (leftPivot > rightPivot)
			{
				return -1;
			}

			if ((rightPivot - leftPivot) == 2
				|| centerPivot == leftPivot
				|| centerPivot == rightPivot)
			{


				if (meshcomponentVector[rightPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = rightPivot;
				}

				if (meshcomponentVector[centerPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = centerPivot;
				}

				if (meshcomponentVector[leftPivot]->GetMaterial()->GetUUID() == materialUUID)
				{
					maxIndex = leftPivot;
				}

				return maxIndex;
			}

			if (elementAtCenter < materialUUID)
			{
				return FindMinMaterialIndex(meshcomponentVector, centerPivot, rightPivot, materialUUID);
			}
			else if (elementAtCenter >= materialUUID)
			{
				return FindMinMaterialIndex(meshcomponentVector, leftPivot, centerPivot, materialUUID);
			}

			return -1;
		}

		int Sorting::FindMaterialInsertionIndex(std::vector<MeshComponent*>& meshcomponentVector, int leftPivot, int rightPivot, uint64 materialUUID)
		{
			// Here we need to find an index at which 
			// the left element is smaller and the right element is bigger than the given shader uuid.

			// Todo: improve this!
			// We do not want to do a linear search.
			for (int i = leftPivot; i < rightPivot; ++i) {
				if (meshcomponentVector[i]->GetMaterial()->GetUUID() > materialUUID) {
					return i;
				}
			}

			return rightPivot;
		}
	};
};