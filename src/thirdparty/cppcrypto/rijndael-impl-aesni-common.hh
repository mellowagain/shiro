/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#ifndef CPPCRYPTO_RIJNDAELIMPL_AESNI_COMMON_H
#define CPPCRYPTO_RIJNDAELIMPL_AESNI_COMMON_H

#include <wmmintrin.h>
#include <smmintrin.h>

namespace cppcrypto
{
	namespace detail
	{

		static inline __m128i aes128_keyexpand(__m128i key)
		{
			key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
			key = _mm_xor_si128(key, _mm_slli_si128(key, 4));
			return _mm_xor_si128(key, _mm_slli_si128(key, 4));
		}

		static inline void KEY_192_ASSIST(__m128i* temp1, __m128i * temp2, __m128i * temp3)
		{
			__m128i temp4;
			*temp2 = _mm_shuffle_epi32(*temp2, 0x55);
			temp4 = _mm_slli_si128(*temp1, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			*temp1 = _mm_xor_si128(*temp1, *temp2);
			*temp2 = _mm_shuffle_epi32(*temp1, 0xff);
			temp4 = _mm_slli_si128(*temp3, 0x4);
			*temp3 = _mm_xor_si128(*temp3, temp4);
			*temp3 = _mm_xor_si128(*temp3, *temp2);
		}

#define mm_shuffle_int32(t1, t2, a, r) \
	f1 = _mm_castsi128_pd(t1); \
	f2 = _mm_castsi128_pd(t2); \
	f3 = _mm_shuffle_pd(f1, f2, a); \
	r = _mm_castpd_si128(f3);

#define KEYGEN192STEPA(idx, rc) \
		temp2 = _mm_aeskeygenassist_si128(temp3, rc); \
		KEY_192_ASSIST(&temp1, &temp2, &temp3); \
		mm_shuffle_int32(rk[idx], temp1, 0, rk[idx]); \
		mm_shuffle_int32(temp1, temp3, 1, rk[idx+1]);

#define KEYGEN192STEPB(idx, rc) \
		temp2 = _mm_aeskeygenassist_si128(temp3, rc); \
		KEY_192_ASSIST(&temp1, &temp2, &temp3); \
		rk[idx] = temp1; \
		rk[idx+1] = temp3;

		static inline void KEY_256_ASSIST_1(__m128i* temp1, __m128i * temp2)
		{
			__m128i temp4;
			*temp2 = _mm_shuffle_epi32(*temp2, 0xff);
			temp4 = _mm_slli_si128(*temp1, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp1 = _mm_xor_si128(*temp1, temp4);
			*temp1 = _mm_xor_si128(*temp1, *temp2);
		}
		static inline void KEY_256_ASSIST_2(__m128i* temp1, __m128i * temp3)
		{
			__m128i temp2, temp4;
			temp4 = _mm_aeskeygenassist_si128(*temp1, 0x0);
			temp2 = _mm_shuffle_epi32(temp4, 0xaa);
			temp4 = _mm_slli_si128(*temp3, 0x4);
			*temp3 = _mm_xor_si128(*temp3, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp3 = _mm_xor_si128(*temp3, temp4);
			temp4 = _mm_slli_si128(temp4, 0x4);
			*temp3 = _mm_xor_si128(*temp3, temp4);
			*temp3 = _mm_xor_si128(*temp3, temp2);
		}

#define KEYGEN256STEP(idx, rc) \
		temp2 = _mm_aeskeygenassist_si128(temp3, rc); \
		KEY_256_ASSIST_1(&temp1, &temp2); \
		rk[idx] = temp1; \
		KEY_256_ASSIST_2(&temp1, &temp3); \
		rk[idx+1] = temp3;

		inline static __m128i mm_blend_int64(__m128i t1, __m128i t2, const int mask)
		{
			__m128d f1 = _mm_castsi128_pd(t1);
			__m128d f2 = _mm_castsi128_pd(t2);
			f1 = _mm_blend_pd(f1, f2, 1);
			return _mm_castpd_si128(f1);
		}

		inline static __m128i mm_blend_shuffle_int64(__m128i t1, __m128i t2, const int mask)
		{
			__m128d f1 = _mm_castsi128_pd(t1);
			__m128d f2 = _mm_castsi128_pd(t2);
			f1 = _mm_blend_pd(f1, f2, 1);
			f1 = _mm_shuffle_pd(f1, f1, 1);
			return _mm_castpd_si128(f1);
		}

	}
}

#endif

