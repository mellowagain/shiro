/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#include "rijndael-impl.hh"
#include <xmmintrin.h>
#include <wmmintrin.h>
#include <immintrin.h>
#include <memory.h>
#include "rijndael-impl-aesni-common.hh"
#include "portability.hh"

//#define CPPCRYPTO_DEBUG

namespace cppcrypto {
    namespace detail {

        static inline void KEY_160_ASSIST(__m128i *temp1, __m128i *temp2, __m128i *temp3) {
            __m128i temp4;
            *temp2 = _mm_shuffle_epi32(*temp2, 0x55);
            temp4  = _mm_slli_si128(*temp1, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            temp4  = _mm_slli_si128(temp4, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            temp4  = _mm_slli_si128(temp4, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            *temp1 = _mm_xor_si128(*temp1, *temp2);
            *temp2 = _mm_shuffle_epi32(*temp1, 0xff);
            *temp3 = _mm_xor_si128(*temp3, *temp2);
        }

#define KEYGEN160STEP(idx, rc, sl, sr, b1, b2)         \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc);      \
    KEY_160_ASSIST(&temp1, &temp2, &temp3);            \
    temp4       = _mm_slli_si128(temp1, sl);           \
    rk[idx]     = _mm_blend_epi32(rk[idx], temp4, b1); \
    temp4       = _mm_srli_si128(temp1, sr);           \
    rk[idx + 1] = _mm_blend_epi32(temp4, temp3, b2);

#define KEYGEN160STEPA(idx, rc)                   \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc); \
    KEY_160_ASSIST(&temp1, &temp2, &temp3);       \
    rk[idx]     = temp1;                          \
    rk[idx + 1] = temp3;

#define KEYGEN224STEPA(idx, rc)                           \
    temp3 = _mm_slli_si128(temp3, 4);                     \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc);         \
    temp3 = _mm_srli_si128(temp3, 4);                     \
    KEY_224_ASSIST_1(&temp1, &temp2);                     \
    temp4       = _mm_slli_si128(temp1, 12);              \
    rk[idx]     = _mm_blend_epi32(rk[idx], temp4, 8);     \
    temp4       = _mm_srli_si128(temp1, 4);               \
    rk[idx + 1] = _mm_blend_epi32(temp4, temp3, 8);       \
    KEY_224_ASSIST_2(&temp1, &temp3);                     \
    temp4       = _mm_slli_si128(temp3, 12);              \
    rk[idx + 1] = _mm_blend_epi32(rk[idx + 1], temp4, 8); \
    temp4       = _mm_srli_si128(temp3, 4);               \
    rk[idx + 2] = _mm_blend_epi32(temp4, temp3, 8);

#define KEYGEN224STEPB(idx, rc)                            \
    temp3 = _mm_slli_si128(temp3, 4);                      \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc);          \
    temp3 = _mm_srli_si128(temp3, 4);                      \
    KEY_224_ASSIST_1(&temp1, &temp2);                      \
    temp4       = _mm_slli_si128(temp1, 8);                \
    rk[idx]     = _mm_blend_epi32(rk[idx], temp4, 12);     \
    temp4       = _mm_srli_si128(temp1, 8);                \
    rk[idx + 1] = _mm_blend_epi32(temp4, temp3, 4);        \
    KEY_224_ASSIST_2(&temp1, &temp3);                      \
    temp4       = _mm_slli_si128(temp3, 8);                \
    rk[idx + 1] = _mm_blend_epi32(rk[idx + 1], temp4, 12); \
    rk[idx + 2] = _mm_srli_si128(temp3, 8);

#define KEYGEN224STEPC(idx, rc)                          \
    temp3 = _mm_slli_si128(temp3, 4);                    \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc);        \
    temp3 = _mm_srli_si128(temp3, 4);                    \
    KEY_224_ASSIST_1(&temp1, &temp2);                    \
    temp4       = _mm_slli_si128(temp1, 4);              \
    rk[idx]     = _mm_blend_epi32(rk[idx], temp4, 0x0E); \
    rk[idx + 1] = _mm_srli_si128(temp1, 12);             \
    KEY_224_ASSIST_2(&temp1, &temp3);                    \
    temp3       = _mm_slli_si128(temp3, 4);              \
    rk[idx + 1] = _mm_blend_epi32(rk[idx + 1], temp3, 0x0E);

#define KEYGEN224STEPD(idx, rc)                   \
    temp2 = _mm_aeskeygenassist_si128(temp3, rc); \
    temp3 = _mm_srli_si128(temp3, 4);             \
    KEY_224_ASSIST_1(&temp1, &temp2);             \
    rk[idx] = temp1;                              \
    KEY_224_ASSIST_2(&temp1, &temp3);             \
    rk[idx + 1] = temp3;

        bool rijndael128_160_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 160 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));

            rk[0] = temp1;
            rk[1] = temp3;
            __m128i temp2, temp4;
            temp3 = _mm_shuffle_epi32(temp3, 0x00);

            KEYGEN160STEP(1, 0x01, 4, 12, 0x0E, 0x02);
            // temp3 = _mm_shuffle_epi32(temp3, 0x00);
            KEYGEN160STEP(2, 0x02, 8, 8, 12, 4);
            KEYGEN160STEP(3, 0x04, 12, 4, 8, 8);
            KEYGEN160STEPA(5, 0x08);
            KEYGEN160STEP(6, 0x10, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(7, 0x20, 8, 8, 12, 4);
            KEYGEN160STEP(8, 0x40, 12, 4, 8, 8);
            KEYGEN160STEPA(10, 0x80);

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x1B);
            KEY_160_ASSIST(&temp1, &temp2, &temp3);
            temp4  = _mm_slli_si128(temp1, 4);
            rk[11] = _mm_blend_epi32(temp4, rk[11], 0x01);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[11]);
                std::swap(rk[1], rk[10]);
                std::swap(rk[2], rk[9]);
                std::swap(rk[3], rk[8]);
                std::swap(rk[4], rk[7]);
                std::swap(rk[5], rk[6]);

                rk[1]  = _mm_aesimc_si128(rk[1]);
                rk[2]  = _mm_aesimc_si128(rk[2]);
                rk[3]  = _mm_aesimc_si128(rk[3]);
                rk[4]  = _mm_aesimc_si128(rk[4]);
                rk[5]  = _mm_aesimc_si128(rk[5]);
                rk[6]  = _mm_aesimc_si128(rk[6]);
                rk[7]  = _mm_aesimc_si128(rk[7]);
                rk[8]  = _mm_aesimc_si128(rk[8]);
                rk[9]  = _mm_aesimc_si128(rk[9]);
                rk[10] = _mm_aesimc_si128(rk[10]);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        static inline void KEY_224_ASSIST_1(__m128i *temp1, __m128i *temp2) {
            __m128i temp4;
            *temp2 = _mm_shuffle_epi32(*temp2, 0xff);
            temp4  = _mm_slli_si128(*temp1, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            temp4  = _mm_slli_si128(temp4, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            temp4  = _mm_slli_si128(temp4, 0x4);
            *temp1 = _mm_xor_si128(*temp1, temp4);
            *temp1 = _mm_xor_si128(*temp1, *temp2);
        }

        static inline void KEY_224_ASSIST_2(__m128i *temp1, __m128i *temp3) {
            __m128i temp2, temp4;
            temp4  = _mm_aeskeygenassist_si128(*temp1, 0x0);
            temp2  = _mm_shuffle_epi32(temp4, 0xaa);
            temp4  = _mm_slli_si128(*temp3, 0x4);
            *temp3 = _mm_xor_si128(*temp3, temp4);
            temp4  = _mm_slli_si128(temp4, 0x4);
            *temp3 = _mm_xor_si128(*temp3, temp4);
            *temp3 = _mm_xor_si128(*temp3, temp2);
        }

        bool rijndael128_224_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 224 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2, temp4;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN224STEPA(1, 0x01);
            KEYGEN224STEPB(3, 0x02);
            KEYGEN224STEPC(5, 0x04);
            KEYGEN224STEPD(7, 0x08);
            KEYGEN224STEPA(8, 0x10);
            KEYGEN224STEPB(10, 0x20);
            KEYGEN224STEPC(12, 0x40);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[13]);
                std::swap(rk[1], rk[12]);
                std::swap(rk[2], rk[11]);
                std::swap(rk[3], rk[10]);
                std::swap(rk[4], rk[9]);
                std::swap(rk[5], rk[8]);
                std::swap(rk[6], rk[7]);

                rk[1]  = _mm_aesimc_si128(rk[1]);
                rk[2]  = _mm_aesimc_si128(rk[2]);
                rk[3]  = _mm_aesimc_si128(rk[3]);
                rk[4]  = _mm_aesimc_si128(rk[4]);
                rk[5]  = _mm_aesimc_si128(rk[5]);
                rk[6]  = _mm_aesimc_si128(rk[6]);
                rk[7]  = _mm_aesimc_si128(rk[7]);
                rk[8]  = _mm_aesimc_si128(rk[8]);
                rk[9]  = _mm_aesimc_si128(rk[9]);
                rk[10] = _mm_aesimc_si128(rk[10]);
                rk[11] = _mm_aesimc_si128(rk[11]);
                rk[12] = _mm_aesimc_si128(rk[12]);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael256_224_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 224 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2, temp4;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN224STEPA(1, 0x01);
            KEYGEN224STEPB(3, 0x02);
            KEYGEN224STEPC(5, 0x04);
            KEYGEN224STEPD(7, 0x08);
            KEYGEN224STEPA(8, 0x10);
            KEYGEN224STEPB(10, 0x20);
            KEYGEN224STEPC(12, 0x40);
            KEYGEN224STEPD(14, 0x80);
            KEYGEN224STEPA(15, 0x1b);
            KEYGEN224STEPB(17, 0x36);
            KEYGEN224STEPC(19, 0x6c);
            KEYGEN224STEPD(21, 0xd8);
            KEYGEN224STEPA(22, 0xab);
            KEYGEN224STEPB(24, 0x4d);
            KEYGEN224STEPC(26, 0x9a);
            KEYGEN224STEPD(28, 0x2f);

            temp3 = _mm_slli_si128(temp3, 4);
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x5e);
            temp3 = _mm_srli_si128(temp3, 4);
            KEY_224_ASSIST_1(&temp1, &temp2);
            temp4  = _mm_slli_si128(temp1, 12);
            rk[29] = _mm_blend_epi32(rk[29], temp4, 8);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[28]);
                std::swap(rk[1], rk[29]);
                std::swap(rk[2], rk[26]);
                std::swap(rk[3], rk[27]);
                std::swap(rk[4], rk[24]);
                std::swap(rk[5], rk[25]);
                std::swap(rk[6], rk[22]);
                std::swap(rk[7], rk[23]);
                std::swap(rk[8], rk[20]);
                std::swap(rk[9], rk[21]);
                std::swap(rk[10], rk[18]);
                std::swap(rk[11], rk[19]);
                std::swap(rk[12], rk[16]);
                std::swap(rk[13], rk[17]);

                for (int i = 2; i < 28; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael256_160_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 160 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));

            rk[0] = temp1;
            rk[1] = temp3;
            __m128i temp2, temp4;
            temp3 = _mm_shuffle_epi32(temp3, 0x00);

            KEYGEN160STEP(1, 0x01, 4, 12, 0x0E, 0x02);
            // temp3 = _mm_shuffle_epi32(temp3, 0x00);
            KEYGEN160STEP(2, 0x02, 8, 8, 12, 4);
            KEYGEN160STEP(3, 0x04, 12, 4, 8, 8);
            KEYGEN160STEPA(5, 0x08);
            KEYGEN160STEP(6, 0x10, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(7, 0x20, 8, 8, 12, 4);
            KEYGEN160STEP(8, 0x40, 12, 4, 8, 8);
            KEYGEN160STEPA(10, 0x80);
            KEYGEN160STEP(11, 0x1B, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(12, 0x36, 8, 8, 12, 4);
            KEYGEN160STEP(13, 0x6c, 12, 4, 8, 8);
            KEYGEN160STEPA(15, 0xd8);
            KEYGEN160STEP(16, 0xab, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(17, 0x4d, 8, 8, 12, 4);
            KEYGEN160STEP(18, 0x9a, 12, 4, 8, 8);
            KEYGEN160STEPA(20, 0x2f);
            KEYGEN160STEP(21, 0x5e, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(22, 0xbc, 8, 8, 12, 4);
            KEYGEN160STEP(23, 0x63, 12, 4, 8, 8);
            KEYGEN160STEPA(25, 0xc6);
            KEYGEN160STEP(26, 0x97, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(27, 0x35, 8, 8, 12, 4);
            KEYGEN160STEP(28, 0x6a, 12, 4, 8, 8);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[28]);
                std::swap(rk[1], rk[29]);
                std::swap(rk[2], rk[26]);
                std::swap(rk[3], rk[27]);
                std::swap(rk[4], rk[24]);
                std::swap(rk[5], rk[25]);
                std::swap(rk[6], rk[22]);
                std::swap(rk[7], rk[23]);
                std::swap(rk[8], rk[20]);
                std::swap(rk[9], rk[21]);
                std::swap(rk[10], rk[18]);
                std::swap(rk[11], rk[19]);
                std::swap(rk[12], rk[16]);
                std::swap(rk[13], rk[17]);

                for (int i = 2; i < 28; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael192_160_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 160 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));

            rk[0] = temp1;
            rk[1] = temp3;
            __m128i temp2, temp4;
            temp3 = _mm_shuffle_epi32(temp3, 0x00);

            KEYGEN160STEP(1, 0x01, 4, 12, 0x0E, 0x02);
            // temp3 = _mm_shuffle_epi32(temp3, 0x00);
            KEYGEN160STEP(2, 0x02, 8, 8, 12, 4);
            KEYGEN160STEP(3, 0x04, 12, 4, 8, 8);
            KEYGEN160STEPA(5, 0x08);
            KEYGEN160STEP(6, 0x10, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(7, 0x20, 8, 8, 12, 4);
            KEYGEN160STEP(8, 0x40, 12, 4, 8, 8);
            KEYGEN160STEPA(10, 0x80);
            KEYGEN160STEP(11, 0x1B, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(12, 0x36, 8, 8, 12, 4);
            KEYGEN160STEP(13, 0x6c, 12, 4, 8, 8);
            KEYGEN160STEPA(15, 0xd8);
            KEYGEN160STEP(16, 0xab, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(17, 0x4d, 8, 8, 12, 4);
            KEYGEN160STEP(18, 0x9a, 12, 4, 8, 8);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[18]);
                std::swap(rk[2], rk[17]);
                std::swap(rk[3], rk[15]);
                std::swap(rk[5], rk[14]);
                std::swap(rk[6], rk[12]);
                std::swap(rk[8], rk[11]);

                __m128i t1 = _mm_blend_epi32(rk[16], rk[19], 3); // rk[1]
                rk[19]     = rk[1];
                rk[1]      = t1;
                t1         = _mm_blend_epi32(rk[13], rk[16], 3); // rk[4]
                rk[16]     = _mm_blend_epi32(rk[19], rk[4], 3);
                __m128i t2 = _mm_blend_epi32(rk[4], rk[7], 3); // rk[13]
                rk[4]      = t1;
                t1         = _mm_blend_epi32(rk[10], rk[13], 3); // rk[7]
                rk[10]     = _mm_blend_epi32(rk[7], rk[10], 3);
                rk[7]      = t1;
                rk[13]     = t2;

                for (int i = 2; i < 18; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2    = _mm_aesimc_si128(rk[1]);
                rk[1] = _mm_blend_epi32(t2, rk[1], 3);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        inline static __m128i mm_blend_swap_int64(__m128i t1, __m128i t2, const int mask) {
            __m128d f1 = _mm_castsi128_pd(t1);
            __m128d f2 = _mm_castsi128_pd(t2);
            f1         = _mm_blend_pd(f2, f1, 1);
            f1         = _mm_shuffle_pd(f1, f1, 1);
            return _mm_castpd_si128(f1);
        }

        bool rijndael192_224_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 224 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2, temp4;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN224STEPA(1, 0x01);
            KEYGEN224STEPB(3, 0x02);
            KEYGEN224STEPC(5, 0x04);
            KEYGEN224STEPD(7, 0x08);
            KEYGEN224STEPA(8, 0x10);
            KEYGEN224STEPB(10, 0x20);
            KEYGEN224STEPC(12, 0x40);
            KEYGEN224STEPD(14, 0x80);
            KEYGEN224STEPA(15, 0x1b);
            KEYGEN224STEPB(17, 0x36);
            KEYGEN224STEPC(19, 0x6c);

            if (direction == block_cipher::decryption) {
                __m128i t1 = mm_blend_swap_int64(rk[1], rk[0], 3); // rk[20]
                __m128i t2 = mm_blend_swap_int64(rk[0], rk[2], 3); // rk[19]
                rk[0]      = mm_blend_swap_int64(rk[20], rk[19], 3);
                __m128i t3 = mm_blend_swap_int64(rk[18], rk[20], 3); // rk[1]
                rk[20]     = t1;
                t1         = mm_blend_swap_int64(rk[2], rk[1], 3); // rk[18]
                rk[1]      = t3;
                rk[2]      = mm_blend_swap_int64(rk[19], rk[18], 3);
                rk[19]     = t2;
                rk[18]     = t1;
                t1         = mm_blend_swap_int64(rk[17], rk[16], 3); // rk[3]
                t2         = mm_blend_swap_int64(rk[15], rk[17], 3); // rk[4]
                rk[17]     = mm_blend_swap_int64(rk[4], rk[3], 3);
                t3         = mm_blend_swap_int64(rk[16], rk[15], 3); // rk[5]
                rk[16]     = mm_blend_swap_int64(rk[3], rk[5], 3);
                rk[15]     = mm_blend_swap_int64(rk[5], rk[4], 3);
                rk[3]      = t1;
                rk[4]      = t2;
                rk[5]      = t3;
                t1         = mm_blend_swap_int64(rk[14], rk[13], 3); // rk[6]
                t2         = mm_blend_swap_int64(rk[12], rk[14], 3); // rk[7]
                t3         = mm_blend_swap_int64(rk[13], rk[12], 3); // rk[8]
                rk[14]     = mm_blend_swap_int64(rk[7], rk[6], 3);
                rk[13]     = mm_blend_swap_int64(rk[6], rk[8], 3);
                rk[12]     = mm_blend_swap_int64(rk[8], rk[7], 3);
                rk[6]      = t1;
                rk[7]      = t2;
                rk[8]      = t3;
                t1         = mm_blend_swap_int64(rk[11], rk[10], 3); // rk[9]
                t2         = mm_blend_swap_int64(rk[9], rk[11], 3);  // rk[10]
                rk[11]     = mm_blend_swap_int64(rk[10], rk[9], 3);
                rk[10]     = t2;
                rk[9]      = t1;

                for (int i = 2; i < 19; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                rk[1]  = _mm_blend_epi32(t2, rk[1], 3);
                t2     = _mm_aesimc_si128(rk[19]);
                rk[19] = _mm_blend_epi32(rk[19], t2, 3);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael160_160_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 160 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));

            rk[0] = temp1;
            rk[1] = temp3;
            __m128i temp2, temp4;
            temp3 = _mm_shuffle_epi32(temp3, 0x00);

            KEYGEN160STEP(1, 0x01, 4, 12, 0x0E, 0x02);
            // temp3 = _mm_shuffle_epi32(temp3, 0x00);
            KEYGEN160STEP(2, 0x02, 8, 8, 12, 4);
            KEYGEN160STEP(3, 0x04, 12, 4, 8, 8);
            KEYGEN160STEPA(5, 0x08);
            KEYGEN160STEP(6, 0x10, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(7, 0x20, 8, 8, 12, 4);
            KEYGEN160STEP(8, 0x40, 12, 4, 8, 8);
            KEYGEN160STEPA(10, 0x80);
            KEYGEN160STEP(11, 0x1B, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(12, 0x36, 8, 8, 12, 4);
            KEYGEN160STEP(13, 0x6c, 12, 4, 8, 8);

            if (direction == block_cipher::decryption) {
                __m128i t1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[14]
                __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[2], 4), rk[0], 1), rk[1], 8),
                                               _MM_SHUFFLE(0, 2, 1, 3));                             // rk[13]
                __m128i t3 = _mm_blend_epi32(_mm_slli_si128(rk[1], 4), _mm_srli_si128(rk[3], 4), 3); // rk[12]
                rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[14], rk[13], 8), _MM_SHUFFLE(2, 1, 0, 3));
                rk[1] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[12], _mm_srli_si128(rk[14], 8), 2), rk[13], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[14] = t1;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[2], _mm_srli_si128(rk[4], 8), 2), rk[3], 1),
                                       _MM_SHUFFLE(0, 3, 2, 1)); // rk[11]
                rk[2]  = _mm_blend_epi32(_mm_slli_si128(rk[11], 4), _mm_srli_si128(rk[13], 4), 3);
                rk[13] = t2;
                t2     = _mm_shuffle_epi32(_mm_blend_epi32(rk[3], rk[4], 7), _MM_SHUFFLE(2, 1, 0, 3)); // rk[10]
                rk[3] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[12], 4), rk[10], 1), rk[11], 8), _MM_SHUFFLE(0, 2, 1, 3));
                rk[12] = t3;
                t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[5], rk[6], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[9]
                rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[10], rk[11], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[11] = t1;
                rk[10] = t2;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[7], 4), rk[6], 8), rk[5], 1),
                                       _MM_SHUFFLE(0, 2, 1, 3));                             // rk[8]
                t2     = _mm_blend_epi32(_mm_slli_si128(rk[6], 4), _mm_srli_si128(rk[8], 4), 3); // rk[7]
                rk[5]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[8], rk[9], 7), _MM_SHUFFLE(2, 1, 0, 3));
                rk[6]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[7], _mm_srli_si128(rk[9], 8), 2), rk[8], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[9]  = t3;
                rk[8]  = t1;
                rk[7]  = t2;

                for (int i = 2; i < 13; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                t3     = _mm_aesimc_si128(rk[13]);
                rk[1]  = _mm_blend_epi32(rk[1], t2, 14);
                rk[13] = _mm_blend_epi32(rk[13], t3, 7);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael160_224_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 224 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2, temp4;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN224STEPA(1, 0x01);
            KEYGEN224STEPB(3, 0x02);
            KEYGEN224STEPC(5, 0x04);
            KEYGEN224STEPD(7, 0x08);
            KEYGEN224STEPA(8, 0x10);
            KEYGEN224STEPB(10, 0x20);
            KEYGEN224STEPC(12, 0x40);
            KEYGEN224STEPD(14, 0x80);
            KEYGEN224STEPA(15, 0x1b);

            if (direction == block_cipher::decryption) {
                __m128i t1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 1), _MM_SHUFFLE(2, 1, 0, 3)); // rk[17]

                __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(_mm_slli_si128(rk[2], 8), rk[0], 7), _MM_SHUFFLE(2, 1, 0, 3)); // rk[16]
                rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[16], rk[17], 1), _MM_SHUFFLE(0, 3, 2, 1));
                __m128i t3 = _mm_shuffle_epi32(_mm_blend_epi32(rk[1], rk[2], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[15]
                rk[1]      = _mm_shuffle_epi32(_mm_blend_epi32(_mm_slli_si128(rk[17], 8), rk[15], 7), _MM_SHUFFLE(2, 1, 0, 3));
                rk[17]     = t1;
                t1         = _mm_shuffle_epi32(_mm_blend_epi32(rk[2], rk[3], 7), _MM_SHUFFLE(2, 1, 0, 3));                    // rk[14]
                __m128i t4 = _mm_shuffle_epi32(_mm_blend_epi32(rk[4], _mm_srli_si128(rk[2], 8), 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[13]
                rk[2]      = _mm_shuffle_epi32(
                    _mm_blend_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[13], 4), rk[15], 8), _mm_slli_si128(rk[14], 4), 2), rk[16], 1),
                    _MM_SHUFFLE(1, 2, 0, 3));

                rk[16] = t2;
                t2     = _mm_shuffle_epi32(
                    _mm_blend_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[3], 4), rk[5], 8), _mm_slli_si128(rk[4], 4), 2), rk[6], 1),
                    _MM_SHUFFLE(1, 2, 0, 3)); // rk[12]
                rk[3]  = _mm_blend_epi32(_mm_srli_si128(rk[14], 4), _mm_slli_si128(rk[12], 4), 8);
                rk[15] = t3;
                t3     = _mm_blend_epi32(_mm_slli_si128(rk[5], 4), _mm_srli_si128(rk[7], 4), 1); // rk[11]
                rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[13], rk[12], 8), _MM_SHUFFLE(2, 1, 0, 3));
                rk[14] = t1;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(rk[6], rk[7], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[10]
                rk[5]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[11], rk[12], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[6]  = _mm_blend_epi32(_mm_slli_si128(rk[10], 4), _mm_srli_si128(rk[12], 4), 1);
                rk[13] = t4;
                rk[12] = t2;
                t2     = _mm_shuffle_epi32(_mm_blend_epi32(rk[8], rk[7], 8), _MM_SHUFFLE(2, 1, 0, 3)); // rk[9]
                t4     = _mm_blend_epi32(_mm_slli_si128(rk[7], 4), _mm_srli_si128(rk[9], 4), 7);       // rk[8]

                rk[7] = _mm_shuffle_epi32(
                    _mm_blend_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[10], 4), rk[8], 8), rk[9], 1), _mm_slli_si128(rk[11], 4), 2),
                    _MM_SHUFFLE(0, 3, 1, 2));
                rk[8]  = t4;
                rk[9]  = t2;
                rk[10] = t1;
                rk[11] = t3;

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 18; i++)
                    printf("swapped rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

                for (int i = 2; i < 16; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                rk[1]  = _mm_blend_epi32(rk[1], t2, 14);
                t1     = _mm_aesimc_si128(rk[16]);
                rk[16] = _mm_blend_epi32(rk[16], t1, 1);
#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 18; i++)
                    printf("imc rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        static inline void rijndael224_swapimc(__m128i *rk) {
            __m128i t1 = _mm_srli_si128(rk[1], 4);                                                     // rk[24]
            __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[23]
            __m128i t3 =
                _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[0], 8), rk[2], 8), rk[3], 3), _MM_SHUFFLE(2, 1, 0, 3)); // rk[22]
            rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[23], rk[22], 8), _MM_SHUFFLE(2, 1, 0, 3));
            __m128i t4 = _mm_shuffle_epi32(_mm_blend_epi32(rk[2], rk[1], 8), _MM_SHUFFLE(2, 1, 0, 3)); // rk[21]
            rk[1]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[21], 8), rk[23], 8), rk[24], 3), _MM_SHUFFLE(2, 1, 0, 3));
            rk[24] = t1;
            t1     = _mm_shuffle_epi32(_mm_blend_epi32(rk[4], rk[5], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[20]
            rk[2]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[21], rk[22], 1), _MM_SHUFFLE(0, 3, 2, 1));
            rk[23] = t2;
            t2 = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[3], 4), rk[4], 1), rk[6], 8), _MM_SHUFFLE(0, 2, 1, 3)); // rk[19]
            rk[3]  = _mm_blend_epi32(_mm_slli_si128(rk[19], 4), _mm_srli_si128(rk[22], 4), 3);
            rk[22] = t3;
            t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[6], rk[5], 8), _MM_SHUFFLE(2, 1, 0, 3)); // rk[18]
            rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[20], rk[19], 8), _MM_SHUFFLE(2, 1, 0, 3));
            rk[21] = t4;
            t4     = _mm_blend_epi32(_mm_slli_si128(rk[5], 4), _mm_srli_si128(rk[8], 4), 3); // rk[17]
            rk[5]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[17], 4), rk[18], 1), rk[20], 8), _MM_SHUFFLE(0, 2, 1, 3));
            rk[20] = t1;
            t1     = _mm_shuffle_epi32(_mm_blend_epi32(rk[7], rk[8], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[16]
            rk[6]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[18], rk[19], 1), _MM_SHUFFLE(0, 3, 2, 1));
            rk[19] = t2;
            t2 =
                _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[7], 8), rk[9], 8), rk[10], 3), _MM_SHUFFLE(2, 1, 0, 3)); // rk[15]
            rk[7]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[16], rk[15], 8), _MM_SHUFFLE(2, 1, 0, 3));
            rk[18] = t3;
            t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[9], rk[8], 8), _MM_SHUFFLE(2, 1, 0, 3)); // rk[14]
            rk[8]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[14], 8), rk[16], 8), rk[17], 3), _MM_SHUFFLE(2, 1, 0, 3));
            rk[17] = t4;
            t4     = _mm_shuffle_epi32(_mm_blend_epi32(rk[11], rk[12], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[13]
            rk[9]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[14], rk[15], 1), _MM_SHUFFLE(0, 3, 2, 1));
            rk[16] = t1;
            t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[10], 4), rk[11], 1), rk[13], 8),
                                   _MM_SHUFFLE(0, 2, 1, 3)); // rk[12]
            rk[10] = _mm_blend_epi32(_mm_slli_si128(rk[12], 4), _mm_srli_si128(rk[15], 4), 3);
            rk[11] = _mm_shuffle_epi32(_mm_blend_epi32(rk[13], rk[12], 8), _MM_SHUFFLE(2, 1, 0, 3));
            rk[12] = t1;
            rk[13] = t4;
            rk[14] = t3;
            rk[15] = t2;

            for (int i = 2; i < 22; i++)
                rk[i] = _mm_aesimc_si128(rk[i]);
            t2     = _mm_aesimc_si128(rk[1]);
            rk[1]  = _mm_blend_epi32(rk[1], t2, 8);
            t1     = _mm_aesimc_si128(rk[22]);
            rk[22] = _mm_blend_epi32(rk[22], t1, 7);
        }

        bool rijndael224_128_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            rk[0]  = _mm_loadu_si128((const __m128i *) key);
            rk[1]  = _mm_xor_si128(aes128_keyexpand(rk[0]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[0], 0x01), 0xff));
            rk[2]  = _mm_xor_si128(aes128_keyexpand(rk[1]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[1], 0x02), 0xff));
            rk[3]  = _mm_xor_si128(aes128_keyexpand(rk[2]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[2], 0x04), 0xff));
            rk[4]  = _mm_xor_si128(aes128_keyexpand(rk[3]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[3], 0x08), 0xff));
            rk[5]  = _mm_xor_si128(aes128_keyexpand(rk[4]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[4], 0x10), 0xff));
            rk[6]  = _mm_xor_si128(aes128_keyexpand(rk[5]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[5], 0x20), 0xff));
            rk[7]  = _mm_xor_si128(aes128_keyexpand(rk[6]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[6], 0x40), 0xff));
            rk[8]  = _mm_xor_si128(aes128_keyexpand(rk[7]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[7], 0x80), 0xff));
            rk[9]  = _mm_xor_si128(aes128_keyexpand(rk[8]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[8], 0x1B), 0xff));
            rk[10] = _mm_xor_si128(aes128_keyexpand(rk[9]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[9], 0x36), 0xff));
            rk[11] = _mm_xor_si128(aes128_keyexpand(rk[10]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[10], 0x6c), 0xff));
            rk[12] = _mm_xor_si128(aes128_keyexpand(rk[11]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[11], 0xd8), 0xff));
            rk[13] = _mm_xor_si128(aes128_keyexpand(rk[12]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[12], 0xab), 0xff));
            rk[14] = _mm_xor_si128(aes128_keyexpand(rk[13]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[13], 0x4d), 0xff));
            rk[15] = _mm_xor_si128(aes128_keyexpand(rk[14]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[14], 0x9a), 0xff));
            rk[16] = _mm_xor_si128(aes128_keyexpand(rk[15]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[15], 0x2f), 0xff));
            rk[17] = _mm_xor_si128(aes128_keyexpand(rk[16]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[16], 0x5e), 0xff));
            rk[18] = _mm_xor_si128(aes128_keyexpand(rk[17]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[17], 0xbc), 0xff));
            rk[19] = _mm_xor_si128(aes128_keyexpand(rk[18]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[18], 0x63), 0xff));
            rk[20] = _mm_xor_si128(aes128_keyexpand(rk[19]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[19], 0xc6), 0xff));
            rk[21] = _mm_xor_si128(aes128_keyexpand(rk[20]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[20], 0x97), 0xff));
            rk[22] = _mm_xor_si128(aes128_keyexpand(rk[21]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[21], 0x35), 0xff));
            rk[23] = _mm_xor_si128(aes128_keyexpand(rk[22]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[22], 0x6a), 0xff));
            rk[24] = _mm_xor_si128(aes128_keyexpand(rk[23]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[23], 0xd4), 0xff));

            if (direction == block_cipher::decryption) {
                rijndael224_swapimc(rk);
            }

            return true;
        }

        bool rijndael224_160_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 160 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));

            rk[0] = temp1;
            rk[1] = temp3;
            __m128i temp2, temp4;
            temp3 = _mm_shuffle_epi32(temp3, 0x00);

            KEYGEN160STEP(1, 0x01, 4, 12, 0x0E, 0x02);
            // temp3 = _mm_shuffle_epi32(temp3, 0x00);
            KEYGEN160STEP(2, 0x02, 8, 8, 12, 4);
            KEYGEN160STEP(3, 0x04, 12, 4, 8, 8);
            KEYGEN160STEPA(5, 0x08);
            KEYGEN160STEP(6, 0x10, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(7, 0x20, 8, 8, 12, 4);
            KEYGEN160STEP(8, 0x40, 12, 4, 8, 8);
            KEYGEN160STEPA(10, 0x80);
            KEYGEN160STEP(11, 0x1B, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(12, 0x36, 8, 8, 12, 4);
            KEYGEN160STEP(13, 0x6c, 12, 4, 8, 8);
            KEYGEN160STEPA(15, 0xd8);
            KEYGEN160STEP(16, 0xab, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(17, 0x4d, 8, 8, 12, 4);
            KEYGEN160STEP(18, 0x9a, 12, 4, 8, 8);
            KEYGEN160STEPA(20, 0x2f);
            KEYGEN160STEP(21, 0x5e, 4, 12, 0x0E, 0x02);
            KEYGEN160STEP(22, 0xbc, 8, 8, 12, 4);
            KEYGEN160STEP(23, 0x63, 12, 4, 8, 8);

            if (direction == block_cipher::decryption) {
                rijndael224_swapimc(rk);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael224_192_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 192 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2;
            rk[0] = temp1;
            rk[1] = temp3;
            __m128d f1, f2, f3;

            KEYGEN192STEPA(1, 0x01);
            KEYGEN192STEPB(3, 0x02);
            KEYGEN192STEPA(4, 0x04);
            KEYGEN192STEPB(6, 0x08);
            KEYGEN192STEPA(7, 0x10);
            KEYGEN192STEPB(9, 0x20);
            KEYGEN192STEPA(10, 0x40);
            KEYGEN192STEPB(12, 0x80);
            KEYGEN192STEPA(13, 0x1b);
            KEYGEN192STEPB(15, 0x36);
            KEYGEN192STEPA(16, 0x6c);
            KEYGEN192STEPB(18, 0xd8);
            KEYGEN192STEPA(19, 0xab);
            KEYGEN192STEPB(21, 0x4d);
            KEYGEN192STEPA(22, 0x9a);

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x2f);
            KEY_192_ASSIST(&temp1, &temp2, &temp3);
            rk[24] = temp1;

            if (direction == block_cipher::decryption) {
                rijndael224_swapimc(rk);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        bool rijndael224_224_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 224 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2, temp4;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN224STEPA(1, 0x01);
            KEYGEN224STEPB(3, 0x02);
            KEYGEN224STEPC(5, 0x04);
            KEYGEN224STEPD(7, 0x08);
            KEYGEN224STEPA(8, 0x10);
            KEYGEN224STEPB(10, 0x20);
            KEYGEN224STEPC(12, 0x40);
            KEYGEN224STEPD(14, 0x80);
            KEYGEN224STEPA(15, 0x1b);
            KEYGEN224STEPB(17, 0x36);
            KEYGEN224STEPC(19, 0x6c);
            KEYGEN224STEPD(21, 0xd8);
            KEYGEN224STEPA(22, 0xab);

            temp3 = _mm_slli_si128(temp3, 4);
            temp2 = _mm_aeskeygenassist_si128(temp3, 0x4d);
            temp3 = _mm_srli_si128(temp3, 4);
            KEY_224_ASSIST_1(&temp1, &temp2);
            temp4  = _mm_slli_si128(temp1, 8);
            rk[24] = _mm_blend_epi32(rk[24], temp4, 12);

            if (direction == block_cipher::decryption) {
                rijndael224_swapimc(rk);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        static inline void rijndael160_ks(int j, __m128i &key1, __m128i &key2, __m128i *rk, int &idx) {
            int type = j % 4;
            if (type == 1) {
                key1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[idx], rk[idx + 1], 1), _MM_SHUFFLE(0, 3, 2, 1));
                key2 = _mm_srli_si128(rk[++idx], 4);
            } else if (type == 2) {
                key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                key2 = _mm_srli_si128(rk[++idx], 8);
            } else if (type == 3) {
                key1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[idx], rk[idx + 1], 7), _MM_SHUFFLE(2, 1, 0, 3));
                key2 = _mm_srli_si128(rk[++idx], 12);
                idx++;
            } else {
                key1 = rk[idx++];
                key2 = rk[idx];
            }
        }

        static inline void rijndael160_encrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK    = _mm_set_epi32(0x00000000, 0x00000000, 0x00000000, 0x80808000);
            __m128i PRESHUFFLE_MASK1  = _mm_set_epi32(0x0f0e0d0c, 0x070a0908, 0x03020504, 0x0b060100);
            __m128i POSTSHUFFLE_MASK2 = _mm_set_epi32(0x03808080, 0x80028080, 0x80800180, 0x80808000);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 160 / 8);
            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);
#ifdef CPPCRYPTO_DEBUG
            printf("initial data1: 0x%016I64x %016I64x\n", data1.m128i_i64[1], data1.m128i_i64[0]);
            printf("initial data2: 0x%016I64x %016I64x\n", data2.m128i_i64[1], data2.m128i_i64[0]);
#endif

            __m128i key1, key2;
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);
#ifdef CPPCRYPTO_DEBUG
            printf("round 0 key1: 0x%016I64x %016I64x\n", rk[0].m128i_i64[1], rk[0].m128i_i64[0]);
            printf("round 0 key2: 0x%016I64x %016I64x\n", rk[1].m128i_i64[1], rk[1].m128i_i64[0]);

            printf("round 0 data1: 0x%016I64x %016I64x\n", data1.m128i_i64[1], data1.m128i_i64[0]);
            printf("round 0 data2: 0x%016I64x %016I64x\n", data2.m128i_i64[1], data2.m128i_i64[0]);
#endif

            int idx = 1, j;
            for (j = 1; j < r; j++) {
                rijndael160_ks(j, key1, key2, rk, idx);

#ifdef CPPCRYPTO_DEBUG
                printf("round %d key1: 0x%016I64x %016I64x\n", j, key1.m128i_i64[1], key1.m128i_i64[0]);
                printf("round %d key2: 0x%016I64x %016I64x\n", j, key2.m128i_i64[1], key2.m128i_i64[0]);
#endif

                data1 = _mm_shuffle_epi8(data1, PRESHUFFLE_MASK1);
                tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
                tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
                tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
                data1 = _mm_aesenc_si128(tmp1, key1);
                data2 = _mm_aesenc_si128(tmp2, key2);

#ifdef CPPCRYPTO_DEBUG
                printf("round %d data1: 0x%016I64x %016I64x\n", j, data1.m128i_i64[1], data1.m128i_i64[0]);
                printf("round %d data2: 0x%016I64x %016I64x\n", j, data2.m128i_i64[1], data2.m128i_i64[0]);
#endif
            }

            rijndael160_ks(j, key1, key2, rk, idx);
#ifdef CPPCRYPTO_DEBUG
            printf("last round key1: 0x%016I64x %016I64x\n", key1.m128i_i64[1], key1.m128i_i64[0]);
            printf("last round key2: 0x%016I64x %016I64x\n", key2.m128i_i64[1], key2.m128i_i64[0]);
#endif

            data1 = _mm_shuffle_epi8(data1, PRESHUFFLE_MASK1);
            tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
            tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
            tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
            tmp1  = _mm_aesenclast_si128(tmp1, key1);
            tmp2  = _mm_aesenclast_si128(tmp2, key2);

#ifdef CPPCRYPTO_DEBUG
            printf("last round data1: 0x%016I64x %016I64x\n", tmp1.m128i_i64[1], tmp1.m128i_i64[0]);
            printf("last round data2: 0x%016I64x %016I64x\n", tmp2.m128i_i64[1], tmp2.m128i_i64[0]);
#endif

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 160 / 8);
            zero_memory(buf, sizeof(buf));
        }

        static inline void rijndael160_decrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK    = _mm_set_epi32(0x00008000, 0x00800000, 0x80000000, 0x00000000);
            __m128i PRESHUFFLE_MASK2  = _mm_set_epi32(0x80800180, 0x80028080, 0x03808080, 0x80808000);
            __m128i POSTSHUFFLE_MASK1 = _mm_set_epi32(0x070a0d0c, 0x0f0e0908, 0x0b060504, 0x03020100);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 160 / 8);
            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);

            __m128i key1, key2;
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);

            int idx = 1, j;
            for (j = 1; j < r; j++) {
                rijndael160_ks(j, key1, key2, rk, idx);
                data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
                tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
                tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
                tmp1  = _mm_shuffle_epi8(tmp1, POSTSHUFFLE_MASK1);
                data1 = _mm_aesdec_si128(tmp1, key1);
                data2 = _mm_aesdec_si128(tmp2, key2);
            }

            rijndael160_ks(j, key1, key2, rk, idx);
            data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
            tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
            tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
            tmp1  = _mm_shuffle_epi8(tmp1, POSTSHUFFLE_MASK1);
            tmp1  = _mm_aesdeclast_si128(tmp1, key1);
            tmp2  = _mm_aesdeclast_si128(tmp2, key2);

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 160 / 8);
            zero_memory(buf, sizeof(buf));
        }

        void rijndael160_128_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_encrypt_block(in, out, 11, rk);
        }

        void rijndael160_128_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_decrypt_block(in, out, 11, rk);
        }

        bool rijndael160_128_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            rk[0]  = _mm_loadu_si128((const __m128i *) key);
            rk[1]  = _mm_xor_si128(aes128_keyexpand(rk[0]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[0], 0x01), 0xff));
            rk[2]  = _mm_xor_si128(aes128_keyexpand(rk[1]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[1], 0x02), 0xff));
            rk[3]  = _mm_xor_si128(aes128_keyexpand(rk[2]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[2], 0x04), 0xff));
            rk[4]  = _mm_xor_si128(aes128_keyexpand(rk[3]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[3], 0x08), 0xff));
            rk[5]  = _mm_xor_si128(aes128_keyexpand(rk[4]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[4], 0x10), 0xff));
            rk[6]  = _mm_xor_si128(aes128_keyexpand(rk[5]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[5], 0x20), 0xff));
            rk[7]  = _mm_xor_si128(aes128_keyexpand(rk[6]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[6], 0x40), 0xff));
            rk[8]  = _mm_xor_si128(aes128_keyexpand(rk[7]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[7], 0x80), 0xff));
            rk[9]  = _mm_xor_si128(aes128_keyexpand(rk[8]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[8], 0x1B), 0xff));
            rk[10] = _mm_xor_si128(aes128_keyexpand(rk[9]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[9], 0x36), 0xff));
            rk[11] = _mm_xor_si128(aes128_keyexpand(rk[10]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[10], 0x6c), 0xff));
            rk[12] = _mm_xor_si128(aes128_keyexpand(rk[11]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[11], 0xd8), 0xff));
            rk[13] = _mm_xor_si128(aes128_keyexpand(rk[12]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[12], 0xab), 0xff));
            rk[14] = _mm_xor_si128(aes128_keyexpand(rk[13]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[13], 0x4d), 0xff));

#ifdef CPPCRYPTO_DEBUG
            for (int i = 0; i < 15; i++)
                printf("assist rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

            if (direction == block_cipher::decryption) {
                __m128i t1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[14]
                __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[2], 4), rk[0], 1), rk[1], 8),
                                               _MM_SHUFFLE(0, 2, 1, 3));                             // rk[13]
                __m128i t3 = _mm_blend_epi32(_mm_slli_si128(rk[1], 4), _mm_srli_si128(rk[3], 4), 3); // rk[12]
                rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[14], rk[13], 8), _MM_SHUFFLE(2, 1, 0, 3));
                rk[1] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[12], _mm_srli_si128(rk[14], 8), 2), rk[13], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[14] = t1;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[2], _mm_srli_si128(rk[4], 8), 2), rk[3], 1),
                                       _MM_SHUFFLE(0, 3, 2, 1)); // rk[11]
                rk[2]  = _mm_blend_epi32(_mm_slli_si128(rk[11], 4), _mm_srli_si128(rk[13], 4), 3);
                rk[13] = t2;
                t2     = _mm_shuffle_epi32(_mm_blend_epi32(rk[3], rk[4], 7), _MM_SHUFFLE(2, 1, 0, 3)); // rk[10]
                rk[3] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[12], 4), rk[10], 1), rk[11], 8), _MM_SHUFFLE(0, 2, 1, 3));
                rk[12] = t3;
                t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[5], rk[6], 1), _MM_SHUFFLE(0, 3, 2, 1)); // rk[9]
                rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[10], rk[11], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[11] = t1;
                rk[10] = t2;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[7], 4), rk[6], 8), rk[5], 1),
                                       _MM_SHUFFLE(0, 2, 1, 3));                             // rk[8]
                t2     = _mm_blend_epi32(_mm_slli_si128(rk[6], 4), _mm_srli_si128(rk[8], 4), 3); // rk[7]
                rk[5]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[8], rk[9], 7), _MM_SHUFFLE(2, 1, 0, 3));
                rk[6]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[7], _mm_srli_si128(rk[9], 8), 2), rk[8], 1), _MM_SHUFFLE(0, 3, 2, 1));
                rk[9]  = t3;
                rk[8]  = t1;
                rk[7]  = t2;

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 15; i++)
                    printf("swapped rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

                for (int i = 2; i < 13; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                t3     = _mm_aesimc_si128(rk[13]);
                rk[1]  = _mm_blend_epi32(rk[1], t2, 14);
                rk[13] = _mm_blend_epi32(rk[13], t3, 7);
#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 15; i++)
                    printf("imc rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif
            }
            return true;
        }

        void rijndael160_160_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_encrypt_block(in, out, 11, rk);
        }

        void rijndael160_160_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_decrypt_block(in, out, 11, rk);
        }

        void rijndael160_192_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_encrypt_block(in, out, 12, rk);
        }

        void rijndael160_192_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_decrypt_block(in, out, 12, rk);
        }

        bool rijndael160_192_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            unsigned char keycopy[32];
            memset(keycopy, 0, sizeof(keycopy));
            memcpy(keycopy, key, 192 / 8);

            __m128i temp1 = _mm_loadu_si128((__m128i *) keycopy);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (keycopy + 16));
            __m128i temp2;
            rk[0] = temp1;
            rk[1] = temp3;
            __m128d f1, f2, f3;

            KEYGEN192STEPA(1, 0x01);
            KEYGEN192STEPB(3, 0x02);
            KEYGEN192STEPA(4, 0x04);
            KEYGEN192STEPB(6, 0x08);
            KEYGEN192STEPA(7, 0x10);
            KEYGEN192STEPB(9, 0x20);
            KEYGEN192STEPA(10, 0x40);
            KEYGEN192STEPB(12, 0x80);
            KEYGEN192STEPA(13, 0x1b);
            KEYGEN192STEPB(15, 0x36);

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x6c);
            KEY_192_ASSIST(&temp1, &temp2, &temp3);
            mm_shuffle_int32(rk[16], temp1, 0, rk[16]);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[15]);
                std::swap(rk[5], rk[10]);

                __m128i t1 = rk[1];                                                                        // rk[16]
                __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(rk[1], rk[2], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[14]
                __m128i t3 = _mm_blend_epi32(rk[3], _mm_slli_si128(rk[1], 8), 8);                          // rk[13]
                rk[1] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[14], 4), rk[16], 1), rk[13], 8), _MM_SHUFFLE(2, 1, 3, 0));
                rk[16] = t1;
                t1     = _mm_blend_epi32(rk[2], _mm_srli_si128(rk[4], 8), 3); // rk[12]
                rk[2]  = _mm_blend_epi32(rk[12], _mm_srli_si128(rk[14], 8), 3);
                rk[14] = t2;
                t2     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[4], 4), rk[6], 1), rk[3], 8),
                                       _MM_SHUFFLE(2, 1, 3, 0)); // rk[11]
                rk[3]  = _mm_blend_epi32(_mm_slli_si128(rk[11], 8), rk[13], 7);
                rk[13] = t3;
                t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[6], rk[7], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[9]
                rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[11], rk[12], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[12] = t1;
                t1     = _mm_blend_epi32(_mm_slli_si128(rk[6], 8), rk[8], 7); // rk[8]
                rk[6]  = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[9], 4), rk[11], 1), rk[8], 8), _MM_SHUFFLE(2, 1, 3, 0));
                rk[7]  = _mm_blend_epi32(rk[7], _mm_srli_si128(rk[9], 8), 3);
                rk[8]  = t1;
                rk[9]  = t3;
                rk[11] = t2;

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 17; i++)
                    printf("swapped rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

                for (int i = 2; i < 15; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2    = _mm_aesimc_si128(rk[1]);
                rk[1] = _mm_blend_epi32(rk[1], t2, 14);
#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 17; i++)
                    printf("imc rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        void rijndael160_224_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_encrypt_block(in, out, 13, rk);
        }

        void rijndael160_224_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_decrypt_block(in, out, 13, rk);
        }

        void rijndael160_256_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_encrypt_block(in, out, 14, rk);
        }

        void rijndael160_256_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael160_decrypt_block(in, out, 14, rk);
        }

        bool rijndael160_256_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            __m128i temp1 = _mm_loadu_si128((__m128i *) key);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (key + 16));
            __m128i temp2;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN256STEP(2, 0x01);
            KEYGEN256STEP(4, 0x02);
            KEYGEN256STEP(6, 0x04);
            KEYGEN256STEP(8, 0x08);
            KEYGEN256STEP(10, 0x10);
            KEYGEN256STEP(12, 0x20);
            KEYGEN256STEP(14, 0x40);
            KEYGEN256STEP(16, 0x80);

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x1b);
            KEY_256_ASSIST_1(&temp1, &temp2);
            rk[18] = temp1;

            if (direction == block_cipher::decryption) {
                std::swap(rk[4], rk[14]);

                __m128i t1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 1), _MM_SHUFFLE(1, 0, 3, 2)); // rk[18]
                __m128i t2 = _mm_blend_epi32(_mm_slli_si128(rk[0], 8), rk[2], 3);                          // rk[17]
                __m128i t3 = _mm_blend_epi32(rk[1], _mm_srli_si128(rk[3], 8), 1);                          // rk[16]
                rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[17], rk[18], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[1]      = _mm_blend_epi32(rk[16], _mm_srli_si128(rk[18], 8), 1);
                rk[18]     = t1;
                t1         = _mm_shuffle_epi32(_mm_blend_epi32(rk[2], rk[3], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[15]
                rk[2]      = _mm_blend_epi32(_mm_slli_si128(rk[15], 8), rk[17], 3);
                rk[17]     = t2;
                t2         = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(rk[5], _mm_srli_si128(rk[3], 8), 2), rk[6], 1),
                                       _MM_SHUFFLE(1, 0, 3, 2)); // rk[13]
                rk[3]      = _mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[16], 8), _mm_srli_si128(rk[15], 8), 3), rk[13], 8);
                rk[16]     = t3;
                t3         = _mm_blend_epi32(_mm_slli_si128(rk[5], 8), rk[7], 3); // rk[12]
                rk[5]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[12], rk[13], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[15]     = t1;
                t1         = _mm_blend_epi32(rk[6], _mm_srli_si128(rk[8], 8), 1); // rk[11]
                rk[6]      = _mm_blend_epi32(rk[11], _mm_srli_si128(rk[13], 8), 1);
                rk[13]     = t2;
                t2         = _mm_shuffle_epi32(_mm_blend_epi32(rk[7], rk[8], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[10]
                rk[7]      = _mm_blend_epi32(_mm_slli_si128(rk[10], 8), rk[12], 3);
                rk[12]     = t3;
                rk[8]      = _mm_blend_epi32(_mm_blend_epi32(_mm_slli_si128(rk[11], 8), _mm_srli_si128(rk[10], 8), 3), rk[8], 8);
                rk[11]     = t1;
                rk[10]     = t2;

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 19; i++)
                    printf("swapped rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

                for (int i = 2; i < 17; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                rk[1]  = _mm_blend_epi32(rk[1], t2, 14);
                t1     = _mm_aesimc_si128(rk[17]);
                rk[17] = _mm_blend_epi32(rk[17], t1, 3);
#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 19; i++)
                    printf("imc rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif
            }

            return true;
        }

        static inline void rijndael224_ks(int j, __m128i &key1, __m128i &key2, __m128i *rk, int &idx) {
            int type = j % 4;
            if (type == 1) {
                key1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[idx], rk[idx + 1], 7), _MM_SHUFFLE(2, 1, 0, 3));
                key2 = _mm_shuffle_epi32(_mm_blend_epi32(rk[idx + 1], rk[idx + 2], 3), _MM_SHUFFLE(2, 1, 0, 3));
                idx += 2;
            } else if (type == 2) {
                key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                key2 = mm_blend_shuffle_int64(rk[idx + 1], rk[idx + 2], 3);
                idx += 2;
            } else if (type == 3) {
                key1 = _mm_shuffle_epi32(_mm_blend_epi32(rk[idx], rk[idx + 1], 1), _MM_SHUFFLE(0, 3, 2, 1));
                key2 = _mm_srli_si128(rk[++idx], 4);
                idx++;
            } else {
                key1 = rk[idx++];
                key2 = rk[idx];
            }
        }

        static inline void rijndael224_encrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK    = _mm_set_epi32(0x00000000, 0x80000000, 0x80800000, 0x80808000);
            __m128i SHUFFLE_MASK1     = _mm_set_epi32(0x030e0d0c, 0x0f0a0908, 0x0b060504, 0x07020100);
            __m128i POSTSHUFFLE_MASK2 = _mm_set_epi32(0x03020180, 0x800a0908, 0x0b800504, 0x07068000);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 224 / 8);
            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);
#ifdef CPPCRYPTO_DEBUG
            printf("initial data1: 0x%016I64x %016I64x\n", data1.m128i_i64[1], data1.m128i_i64[0]);
            printf("initial data2: 0x%016I64x %016I64x\n", data2.m128i_i64[1], data2.m128i_i64[0]);
#endif

            __m128i key1, key2;
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);
#ifdef CPPCRYPTO_DEBUG
            printf("round 0 key1: 0x%016I64x %016I64x\n", rk[0].m128i_i64[1], rk[0].m128i_i64[0]);
            printf("round 0 key2: 0x%016I64x %016I64x\n", rk[1].m128i_i64[1], rk[1].m128i_i64[0]);

            printf("round 0 data1: 0x%016I64x %016I64x\n", data1.m128i_i64[1], data1.m128i_i64[0]);
            printf("round 0 data2: 0x%016I64x %016I64x\n", data2.m128i_i64[1], data2.m128i_i64[0]);
#endif

            int idx = 1, j;
            for (j = 1; j < r; j++) {
                rijndael224_ks(j, key1, key2, rk, idx);

#ifdef CPPCRYPTO_DEBUG
                printf("round %d key1: 0x%016I64x %016I64x\n", j, key1.m128i_i64[1], key1.m128i_i64[0]);
                printf("round %d key2: 0x%016I64x %016I64x\n", j, key2.m128i_i64[1], key2.m128i_i64[0]);
#endif

                data1 = _mm_shuffle_epi8(data1, SHUFFLE_MASK1);
                tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
                tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
                tmp1  = _mm_shuffle_epi8(tmp1, SHUFFLE_MASK1);
                tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
                data1 = _mm_aesenc_si128(tmp1, key1);
                data2 = _mm_aesenc_si128(tmp2, key2);

#ifdef CPPCRYPTO_DEBUG
                printf("round %d data1: 0x%016I64x %016I64x\n", j, data1.m128i_i64[1], data1.m128i_i64[0]);
                printf("round %d data2: 0x%016I64x %016I64x\n", j, data2.m128i_i64[1], data2.m128i_i64[0]);
#endif
            }

            rijndael224_ks(j, key1, key2, rk, idx);
#ifdef CPPCRYPTO_DEBUG
            printf("last round key1: 0x%016I64x %016I64x\n", key1.m128i_i64[1], key1.m128i_i64[0]);
            printf("last round key2: 0x%016I64x %016I64x\n", key2.m128i_i64[1], key2.m128i_i64[0]);
#endif

            data1 = _mm_shuffle_epi8(data1, SHUFFLE_MASK1);
            tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
            tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
            tmp1  = _mm_shuffle_epi8(tmp1, SHUFFLE_MASK1);
            tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
            tmp1  = _mm_aesenclast_si128(tmp1, key1);
            tmp2  = _mm_aesenclast_si128(tmp2, key2);

#ifdef CPPCRYPTO_DEBUG
            printf("last round data1: 0x%016I64x %016I64x\n", tmp1.m128i_i64[1], tmp1.m128i_i64[0]);
            printf("last round data2: 0x%016I64x %016I64x\n", tmp2.m128i_i64[1], tmp2.m128i_i64[0]);
#endif

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 224 / 8);
            zero_memory(buf, sizeof(buf));
        }

        static inline void rijndael224_decrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK   = _mm_set_epi32(0x80808000, 0x80800000, 0x80000000, 0x00000000);
            __m128i SHUFFLE_MASK1    = _mm_set_epi32(0x0b0e0d0c, 0x070a0908, 0x03060504, 0x0f020100);
            __m128i PRESHUFFLE_MASK2 = _mm_set_epi32(0x0b0a0980, 0x07068008, 0x03800504, 0x80020100);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 224 / 8);
            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);

            __m128i key1, key2;
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);

            int idx = 1, j;
            for (j = 1; j < r; j++) {
                rijndael224_ks(j, key1, key2, rk, idx);

                data1 = _mm_shuffle_epi8(data1, SHUFFLE_MASK1);
                data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
                tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
                tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
                tmp1  = _mm_shuffle_epi8(tmp1, SHUFFLE_MASK1);
                data1 = _mm_aesdec_si128(tmp1, key1);
                data2 = _mm_aesdec_si128(tmp2, key2);
            }

            rijndael224_ks(j, key1, key2, rk, idx);
            data1 = _mm_shuffle_epi8(data1, SHUFFLE_MASK1);
            data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
            tmp1  = _mm_blendv_epi8(data1, data2, VEC_BLEND_MASK);
            tmp2  = _mm_blendv_epi8(data2, data1, VEC_BLEND_MASK);
            tmp1  = _mm_shuffle_epi8(tmp1, SHUFFLE_MASK1);
            tmp1  = _mm_aesdeclast_si128(tmp1, key1);
            tmp2  = _mm_aesdeclast_si128(tmp2, key2);

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 224 / 8);
            zero_memory(buf, sizeof(buf));
        }

        void rijndael224_128_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael224_encrypt_block(in, out, 13, rk);
        }

        void rijndael224_128_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael224_decrypt_block(in, out, 13, rk);
        }

        void rijndael224_256_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael224_encrypt_block(in, out, 14, rk);
        }

        void rijndael224_256_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael224_decrypt_block(in, out, 14, rk);
        }

        bool rijndael224_256_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
            __m128i temp1 = _mm_loadu_si128((__m128i *) key);
            __m128i temp3 = _mm_loadu_si128((__m128i *) (key + 16));
            __m128i temp2;
            rk[0] = temp1;
            rk[1] = temp3;

            KEYGEN256STEP(2, 0x01);
            KEYGEN256STEP(4, 0x02);
            KEYGEN256STEP(6, 0x04);
            KEYGEN256STEP(8, 0x08);
            KEYGEN256STEP(10, 0x10);
            KEYGEN256STEP(12, 0x20);
            KEYGEN256STEP(14, 0x40);
            KEYGEN256STEP(16, 0x80);
            KEYGEN256STEP(18, 0x1b);
            KEYGEN256STEP(20, 0x36);
            KEYGEN256STEP(22, 0x6c);
            KEYGEN256STEP(24, 0xd8);
            temp2 = _mm_aeskeygenassist_si128(temp3, 0xab);
            KEY_256_ASSIST_1(&temp1, &temp2);
            rk[26] = temp1;

            if (direction == block_cipher::decryption) {
                std::swap(rk[6], rk[20]);
                std::swap(rk[2], rk[23]);
                std::swap(rk[9], rk[16]);

                __m128i t1 = _mm_srli_si128(rk[1], 8);                                                     // rk[26]
                __m128i t2 = _mm_shuffle_epi32(_mm_blend_epi32(rk[0], rk[1], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[25]
                __m128i t3 = _mm_blend_epi32(_mm_slli_si128(rk[0], 8), rk[3], 3);                          // rk[24]
                __m128i t4 = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[4], 4), rk[5], 1), rk[1], 8),
                                               _MM_SHUFFLE(3, 0, 2, 1)); // rk[22]
                rk[0]      = _mm_shuffle_epi32(_mm_blend_epi32(rk[24], rk[25], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[1] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[25], 4), rk[26], 1), rk[22], 8), _MM_SHUFFLE(3, 0, 2, 1));
                rk[26] = t1;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(rk[3], rk[4], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[21]
                rk[3]  = _mm_blend_epi32(_mm_slli_si128(rk[21], 8), rk[24], 3);
                rk[25] = t2;
                t2     = _mm_blend_epi32(_mm_srli_si128(rk[8], 8), rk[5], 14); // rk[19]
                rk[4]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[21], rk[22], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[24] = t3;
                t3     = _mm_shuffle_epi32(_mm_blend_epi32(rk[7], rk[8], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[18]
                rk[5]  = _mm_blend_epi32(_mm_srli_si128(rk[22], 8), rk[19], 14);
                rk[22] = t4;
                t4     = _mm_blend_epi32(_mm_slli_si128(rk[7], 8), rk[10], 3); // rk[17]
                rk[7]  = _mm_shuffle_epi32(_mm_blend_epi32(rk[17], rk[18], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[21] = t1;
                t1     = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[11], 4), rk[12], 1), rk[8], 8),
                                       _MM_SHUFFLE(3, 0, 2, 1)); // rk[15]
                rk[8] = _mm_shuffle_epi32(_mm_blend_epi32(_mm_blend_epi32(_mm_srli_si128(rk[18], 4), rk[19], 1), rk[15], 8), _MM_SHUFFLE(3, 0, 2, 1));
                rk[19] = t2;
                t2     = _mm_shuffle_epi32(_mm_blend_epi32(rk[10], rk[11], 3), _MM_SHUFFLE(1, 0, 3, 2)); // rk[14]
                rk[10] = _mm_blend_epi32(_mm_slli_si128(rk[14], 8), rk[17], 3);
                rk[18] = t3;
                rk[11] = _mm_shuffle_epi32(_mm_blend_epi32(rk[14], rk[15], 3), _MM_SHUFFLE(1, 0, 3, 2));
                rk[12] = _mm_blend_epi32(_mm_srli_si128(rk[15], 8), rk[12], 14);
                rk[17] = t4;
                rk[15] = t1;
                rk[14] = t2;

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 27; i++)
                    printf("swapped rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif

                for (int i = 2; i < 24; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2     = _mm_aesimc_si128(rk[1]);
                rk[1]  = _mm_blend_epi32(rk[1], t2, 8);
                t1     = _mm_aesimc_si128(rk[24]);
                rk[24] = _mm_blend_epi32(rk[24], t1, 3);

#ifdef CPPCRYPTO_DEBUG
                for (int i = 0; i < 27; i++)
                    printf("imc rk[%d]: 0x%016I64x %016I64x\n", i, rk[i].m128i_i64[1], rk[i].m128i_i64[0]);
#endif
            }

            return true;
        }
    }
}
