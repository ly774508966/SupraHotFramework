#include "Octree.h"
#include "Entity.h"

namespace SupraHot
{
	namespace Graphics
	{
		template <class T>
		Octree<T>* Octree<T>::GetRoot()
		{
			static Octree<T>* instance(new Octree<T>);
			return instance;
		}

		template <class T>
		Octree<T>::Octree()
		{
		}

		template <class T>
		Octree<T>::~Octree()
		{
		}

		template <class T>
		uint32 Octree<T>::GetIndex(Vec3 lookUpPosition, Vec3 nodePosition)
		{
			uint32 index = 0;

			if (lookUpPosition.y > nodePosition.y)
			{
				index |= OctreeIndex::TOP;
			}
			else
			{
				index |= OctreeIndex::BOTTOM;
			}

			if (lookUpPosition.x < nodePosition.x)
			{
				index |= OctreeIndex::LEFT;
			}
			else
			{
				index |= OctreeIndex::RIGHT;
			}

			if (lookUpPosition.z < nodePosition.z)
			{
				index |= OctreeIndex::BACK;
			}
			else
			{
				index |= OctreeIndex::FRONT;
			}

			return index;
		}

		template class Octree<Vec3>;
		template class Octree<Entity*>;
		template class Octree<MeshComponent*>;
	}
}
