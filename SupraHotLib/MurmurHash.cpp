#include "MurmurHash.h"
#include <stdlib.h>
#include <stdint.h>


namespace SupraHot
{
	namespace Utils
	{
		namespace MurmurHash
		{

#ifdef PLATFORM_WINDOWS
	#define FORCE_INLINE    __forceinline
	#define ROTL32(x,y)     _rotl(x,y)
	#define ROTL64(x,y)     _rotl64(x,y)
#else
	#define FORCE_INLINE    inline
	static FORCE_INLINE uint32_t rotl32(uint32_t x, int8_t r)
	{
		return (x << r) | (x >> (32 - r));
	}

	static FORCE_INLINE uint64_t rotl64(uint64_t x, int8_t r)
	{
		return (x << r) | (x >> (64 - r));
	}

#define ROTL32(x,y)     rotl32(x,y)
#define ROTL64(x,y)     rotl64(x,y)

#endif

	#define BIG_CONSTANT(x) (x)
			
			std::string Hash::ToString() const
			{
				return std::to_string(A) + "_" + std::to_string(B);
			}


			//-----------------------------------------------------------------------------
			// Block read - if your platform needs to do endian-swapping or can only
			// handle aligned reads, do the conversion here

			FORCE_INLINE uint32_t getblock(const uint32_t * p, int i)
			{
				return p[i];
			}

			FORCE_INLINE uint64_t getblock(const uint64_t * p, int i)
			{
				return p[i];
			}

			//-----------------------------------------------------------------------------
			// Finalization mix - force all bits of a hash block to avalanche

			FORCE_INLINE uint32_t fmix(uint32_t h)
			{
				h ^= h >> 16;
				h *= 0x85ebca6b;
				h ^= h >> 13;
				h *= 0xc2b2ae35;
				h ^= h >> 16;

				return h;
			}

			//----------

			FORCE_INLINE uint64_t fmix(uint64_t k)
			{
				k ^= k >> 33;
				k *= BIG_CONSTANT(0xff51afd7ed558ccd);
				k ^= k >> 33;
				k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
				k ^= k >> 33;

				return k;
			}


			//-----------------------------------------------------------------------------

			Hash GenerateHash(const void* key, const int len, const uint32_t seed)
			{
				const uint8_t * data = (const uint8_t*)key;
				const int nblocks = len / 16;

				uint64_t h1 = seed;
				uint64_t h2 = seed;

				const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
				const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

				//----------
				// body

				const uint64_t * blocks = (const uint64_t *)(data);

				for (int i = 0; i < nblocks; i++)
				{
					uint64_t k1 = getblock(blocks, i * 2 + 0);
					uint64_t k2 = getblock(blocks, i * 2 + 1);

					k1 *= c1; k1 = ROTL64(k1, 31); k1 *= c2; h1 ^= k1;

					h1 = ROTL64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

					k2 *= c2; k2 = ROTL64(k2, 33); k2 *= c1; h2 ^= k2;

					h2 = ROTL64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
				}

				//----------
				// tail

				const uint8_t * tail = (const uint8_t*)(data + nblocks * 16);

				uint64_t k1 = 0;
				uint64_t k2 = 0;

				switch (len & 15)
				{
				case 15: k2 ^= uint64_t(tail[14]) << 48;
				case 14: k2 ^= uint64_t(tail[13]) << 40;
				case 13: k2 ^= uint64_t(tail[12]) << 32;
				case 12: k2 ^= uint64_t(tail[11]) << 24;
				case 11: k2 ^= uint64_t(tail[10]) << 16;
				case 10: k2 ^= uint64_t(tail[9]) << 8;
				case  9: k2 ^= uint64_t(tail[8]) << 0;
					k2 *= c2; k2 = ROTL64(k2, 33); k2 *= c1; h2 ^= k2;

				case  8: k1 ^= uint64_t(tail[7]) << 56;
				case  7: k1 ^= uint64_t(tail[6]) << 48;
				case  6: k1 ^= uint64_t(tail[5]) << 40;
				case  5: k1 ^= uint64_t(tail[4]) << 32;
				case  4: k1 ^= uint64_t(tail[3]) << 24;
				case  3: k1 ^= uint64_t(tail[2]) << 16;
				case  2: k1 ^= uint64_t(tail[1]) << 8;
				case  1: k1 ^= uint64_t(tail[0]) << 0;
					k1 *= c1; k1 = ROTL64(k1, 31); k1 *= c2; h1 ^= k1;
				};

				//----------
				// finalization

				h1 ^= len; h2 ^= len;

				h1 += h2;
				h2 += h1;

				h1 = fmix(h1);
				h2 = fmix(h2);

				h1 += h2;
				h2 += h1;

				return Hash(h1, h2);
			}

			Hash GenerateHash_32(const void* key, int len, uint32 seed)
			{
				const uint8_t * data = (const uint8_t*)key;

				uint32_t h = seed;
				if (len > 3) {
					const uint32_t* key_x4 = (const uint32_t*)data;
					size_t i = len >> 2;
					do {
						uint32_t k = *key_x4++;
						k *= 0xcc9e2d51;
						k = (k << 15) | (k >> 17);
						k *= 0x1b873593;
						h ^= k;
						h = (h << 13) | (h >> 19);
						h += (h << 2) + 0xe6546b64;
					} while (--i);
					data = (const uint8_t*)key_x4;
				}
				if (len & 3) {
					size_t i = len & 3;
					uint32_t k = 0;
					data = &data[i - 1];
					do {
						k <<= 8;
						k |= *data--;
					} while (--i);
					k *= 0xcc9e2d51;
					k = (k << 15) | (k >> 17);
					k *= 0x1b873593;
					h ^= k;
				}
				h ^= len;
				h ^= h >> 16;
				h *= 0x85ebca6b;
				h ^= h >> 13;
				h *= 0xc2b2ae35;
				h ^= h >> 16;
				return Hash(h, 0);
			}
		};
	};
};