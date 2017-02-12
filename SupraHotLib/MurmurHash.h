#pragma once
#include "Platform.h"

namespace SupraHot
{
	namespace Utils
	{
		namespace MurmurHash
		{
			
			struct Hash
			{
				uint64 A;
				uint64 B;

				Hash() : A(0), B(0) {}
				Hash(uint64 a, uint64 b) : A(a), B(b) {}

				std::string ToString() const;

				bool operator==(const Hash& other)
				{
					return A == other.A && B == other.B;
				}
			};

			Hash GenerateHash(const void* key, int len, uint32 seed = 0);
			Hash GenerateHash_32(const void* key, int len, uint32 seed = 0);

		};
	};
};