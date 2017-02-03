#pragma once
#include "Platform.h"
#include "Vec3.h"
#include <vector>

namespace SupraHot
{
	namespace Graphics
	{
		using namespace Math;

		#define BITSHIFT(num) (1 << num)
		enum OctreeIndex
		{

			// 0 0 0
			// 0 0 1
			

			TOP = BITSHIFT(0),
			BOTTOM = BITSHIFT(1),
			FRONT = BITSHIFT(2),
			BACK = BITSHIFT(3),
			LEFT = BITSHIFT(4),
			RIGHT = BITSHIFT(5),

			TOP_FRONT_LEFT = TOP | FRONT | LEFT,
			TOP_FRONT_RIGHT = TOP | FRONT | RIGHT,
			TOP_BACK_RIGHT = TOP | BACK | RIGHT,
			TOP_BACK_LEFT = TOP | BACK | LEFT,

			BOTTOM_FRONT_LEFT = BOTTOM | FRONT | LEFT,
			BOTTOM_FRONT_RIGHT = BOTTOM | FRONT | RIGHT,
			BOTTOM_BACK_RIGHT = BOTTOM | BACK | RIGHT,
			BOTTOM_BACK_LEFT = BOTTOM | BACK | LEFT

		};

		template<class T>
		class Octree
		{
		private:
			float HalfDimenionLength;
			Vec3 Position;
		public:
			static Octree* GetRoot();
			Octree();
			~Octree();
			uint32 GetIndex(Vec3 lookUpPosition, Vec3 nodePosition);
		};

	};
};