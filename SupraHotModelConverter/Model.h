#pragma once
#include "Vectors.h"
#include <vector>

typedef unsigned char		uint8;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned long long	uint64;

typedef signed char			int8;
typedef short				int16;
typedef int					int32;
typedef long long			int64;

#define BITFIELD(num) (1 << num)

namespace SupraHot
{
	namespace Graphics
	{
		struct Vertex
		{
			Math::Vec3 Position;
			Math::Vec3 Normal;
			Math::Vec2 UV;
			Math::Vec3 Tangent;
			Math::Vec3 BiNormal;
		};

		enum VertexBitfield
		{
			POSITION = BITFIELD(0),
			NORMAL = BITFIELD(1),
			UV = BITFIELD(2),
			TANGENT = BITFIELD(3),
			BINORMAL = BITFIELD(4),
		};

		struct Mesh
		{
			uint32 VertexCount;
			uint32 IndexCount;
			uint32 VertexStride;
			uint32 Attributes;
			std::vector<uint8> Vertices;
			std::vector<uint8> Indices;
		};

	};
};