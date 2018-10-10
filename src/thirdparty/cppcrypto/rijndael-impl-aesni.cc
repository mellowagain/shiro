/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#include "rijndael-impl.hh"
#include <wmmintrin.h>
#include <smmintrin.h>
#include <memory.h>
#include "rijndael-impl-aesni-common.hh"
#include "portability.hh"

//#define CPPCRYPTO_DEBUG

namespace cppcrypto {
    namespace detail {
        bool rijndael128_128_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[10]);
                std::swap(rk[1], rk[9]);
                std::swap(rk[2], rk[8]);
                std::swap(rk[3], rk[7]);
                std::swap(rk[4], rk[6]);

                rk[1] = _mm_aesimc_si128(rk[1]);
                rk[2] = _mm_aesimc_si128(rk[2]);
                rk[3] = _mm_aesimc_si128(rk[3]);
                rk[4] = _mm_aesimc_si128(rk[4]);
                rk[5] = _mm_aesimc_si128(rk[5]);
                rk[6] = _mm_aesimc_si128(rk[6]);
                rk[7] = _mm_aesimc_si128(rk[7]);
                rk[8] = _mm_aesimc_si128(rk[8]);
                rk[9] = _mm_aesimc_si128(rk[9]);
            }

            return true;
        }

        void rijndael128_128_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[1]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[2]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[3]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[4]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[5]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[6]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[7]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[8]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[9]);
            xmm15 = _mm_aesenclast_si128(xmm15, rk[10]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_128_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[1]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[2]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[3]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[4]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[5]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[6]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[7]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[8]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[9]);
            xmm15 = _mm_aesdeclast_si128(xmm15, rk[10]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_160_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[1]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[2]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[3]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[4]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[5]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[6]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[7]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[8]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[9]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[10]);
            xmm15 = _mm_aesenclast_si128(xmm15, rk[11]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_160_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[1]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[2]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[3]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[4]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[5]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[6]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[7]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[8]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[9]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[10]);
            xmm15 = _mm_aesdeclast_si128(xmm15, rk[11]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        bool rijndael128_192_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x80);
            KEY_192_ASSIST(&temp1, &temp2, &temp3);
            rk[12] = temp1;

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[12]);
                std::swap(rk[1], rk[11]);
                std::swap(rk[2], rk[10]);
                std::swap(rk[3], rk[9]);
                std::swap(rk[4], rk[8]);
                std::swap(rk[5], rk[7]);

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
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        void rijndael128_192_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[1]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[2]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[3]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[4]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[5]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[6]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[7]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[8]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[9]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[10]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[11]);
            xmm15 = _mm_aesenclast_si128(xmm15, rk[12]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_192_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[1]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[2]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[3]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[4]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[5]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[6]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[7]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[8]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[9]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[10]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[11]);
            xmm15 = _mm_aesdeclast_si128(xmm15, rk[12]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        bool rijndael128_256_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x40);
            KEY_256_ASSIST_1(&temp1, &temp2);
            rk[14] = temp1;

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[14]);
                std::swap(rk[1], rk[13]);
                std::swap(rk[2], rk[12]);
                std::swap(rk[3], rk[11]);
                std::swap(rk[4], rk[10]);
                std::swap(rk[5], rk[9]);
                std::swap(rk[6], rk[8]);

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
                rk[13] = _mm_aesimc_si128(rk[13]);
            }

            return true;
        }

        void rijndael128_256_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[1]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[2]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[3]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[4]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[5]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[6]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[7]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[8]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[9]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[10]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[11]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[12]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[13]);
            xmm15 = _mm_aesenclast_si128(xmm15, rk[14]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_256_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[1]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[2]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[3]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[4]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[5]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[6]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[7]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[8]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[9]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[10]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[11]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[12]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[13]);
            xmm15 = _mm_aesdeclast_si128(xmm15, rk[14]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_224_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[1]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[2]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[3]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[4]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[5]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[6]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[7]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[8]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[9]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[10]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[11]);
            xmm15 = _mm_aesenc_si128(xmm15, rk[12]);
            xmm15 = _mm_aesenclast_si128(xmm15, rk[13]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        void rijndael128_224_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i xmm15 = _mm_loadu_si128((const __m128i *) in);

            xmm15 = _mm_xor_si128(xmm15, rk[0]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[1]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[2]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[3]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[4]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[5]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[6]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[7]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[8]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[9]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[10]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[11]);
            xmm15 = _mm_aesdec_si128(xmm15, rk[12]);
            xmm15 = _mm_aesdeclast_si128(xmm15, rk[13]);
            _mm_storeu_si128((__m128i *) out, xmm15);
        }

        bool rijndael256_256_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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
            KEYGEN256STEP(26, 0xab);
            KEYGEN256STEP(28, 0x4d);

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

            return true;
        }

        void rijndael256_256_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i RIJNDAEL256_MASK = _mm_set_epi32(0x03020d0c, 0x0f0e0908, 0x0b0a0504, 0x07060100);
            __m128i BLEND_MASK       = _mm_set_epi32(0x80000000, 0x80800000, 0x80800000, 0x80808000);
            int     j;

            data1 = _mm_loadu_si128(&((__m128i *) in)[0]);
            data2 = _mm_loadu_si128(&((__m128i *) in)[1]);
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);
            for (j = 1; j < 14; j++) {
                tmp1  = _mm_blendv_epi8(data1, data2, BLEND_MASK);
                tmp2  = _mm_blendv_epi8(data2, data1, BLEND_MASK);
                tmp1  = _mm_shuffle_epi8(tmp1, RIJNDAEL256_MASK);
                tmp2  = _mm_shuffle_epi8(tmp2, RIJNDAEL256_MASK);
                data1 = _mm_aesenc_si128(tmp1, rk[j * 2]);
                data2 = _mm_aesenc_si128(tmp2, rk[j * 2 + 1]);
            }

            tmp1 = _mm_blendv_epi8(data1, data2, BLEND_MASK);
            tmp2 = _mm_blendv_epi8(data2, data1, BLEND_MASK);
            tmp1 = _mm_shuffle_epi8(tmp1, RIJNDAEL256_MASK);
            tmp2 = _mm_shuffle_epi8(tmp2, RIJNDAEL256_MASK);
            tmp1 = _mm_aesenclast_si128(tmp1, rk[j * 2 + 0]);
            tmp2 = _mm_aesenclast_si128(tmp2, rk[j * 2 + 1]);
            _mm_storeu_si128(&((__m128i *) out)[0], tmp1);
            _mm_storeu_si128(&((__m128i *) out)[1], tmp2);
        }

        void rijndael256_256_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i RIJNDAEL256_MASK_INV = _mm_set_epi32(0x0b0a0d0c, 0x07060908, 0x03020504, 0x0f0e0100);
            __m128i BLEND_MASK_INV       = _mm_set_epi32(0x80808000, 0x80800000, 0x80800000, 0x80000000);
            int     j;

            data1 = _mm_loadu_si128(&((__m128i *) in)[0]);
            data2 = _mm_loadu_si128(&((__m128i *) in)[1]);
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);
            for (j = 1; j < 14; j++) {
                tmp1  = _mm_blendv_epi8(data1, data2, BLEND_MASK_INV);
                tmp2  = _mm_blendv_epi8(data2, data1, BLEND_MASK_INV);
                tmp1  = _mm_shuffle_epi8(tmp1, RIJNDAEL256_MASK_INV);
                tmp2  = _mm_shuffle_epi8(tmp2, RIJNDAEL256_MASK_INV);
                data1 = _mm_aesdec_si128(tmp1, rk[j * 2]);
                data2 = _mm_aesdec_si128(tmp2, rk[j * 2 + 1]);
            }

            tmp1 = _mm_blendv_epi8(data1, data2, BLEND_MASK_INV);
            tmp2 = _mm_blendv_epi8(data2, data1, BLEND_MASK_INV);
            tmp1 = _mm_shuffle_epi8(tmp1, RIJNDAEL256_MASK_INV);
            tmp2 = _mm_shuffle_epi8(tmp2, RIJNDAEL256_MASK_INV);
            tmp1 = _mm_aesdeclast_si128(tmp1, rk[j * 2 + 0]);
            tmp2 = _mm_aesdeclast_si128(tmp2, rk[j * 2 + 1]);
            _mm_storeu_si128(&((__m128i *) out)[0], tmp1);
            _mm_storeu_si128(&((__m128i *) out)[1], tmp2);
        }

        bool rijndael256_128_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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
            rk[25] = _mm_xor_si128(aes128_keyexpand(rk[24]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[24], 0xb3), 0xff));
            rk[26] = _mm_xor_si128(aes128_keyexpand(rk[25]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[25], 0x7d), 0xff));
            rk[27] = _mm_xor_si128(aes128_keyexpand(rk[26]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[26], 0xfa), 0xff));
            rk[28] = _mm_xor_si128(aes128_keyexpand(rk[27]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[27], 0xef), 0xff));
            rk[29] = _mm_xor_si128(aes128_keyexpand(rk[28]), _mm_shuffle_epi32(_mm_aeskeygenassist_si128(rk[28], 0xc5), 0xff));

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
            return true;
        }

        bool rijndael256_192_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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
            KEYGEN192STEPB(24, 0x2f);
            KEYGEN192STEPA(25, 0x5e);
            KEYGEN192STEPB(27, 0xbc);
            KEYGEN192STEPA(28, 0x63);

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

        bool rijndael192_128_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[18]);
                std::swap(rk[2], rk[17]);
                std::swap(rk[3], rk[15]);
                std::swap(rk[5], rk[14]);
                std::swap(rk[6], rk[12]);
                std::swap(rk[8], rk[11]);

                __m128i t1 = mm_blend_int64(rk[16], rk[19], 3); // rk[1]
                rk[19]     = rk[1];
                rk[1]      = t1;
                t1         = mm_blend_int64(rk[13], rk[16], 3); // rk[4]
                rk[16]     = mm_blend_int64(rk[19], rk[4], 3);
                __m128i t2 = mm_blend_int64(rk[4], rk[7], 3); // rk[13]
                rk[4]      = t1;
                t1         = mm_blend_int64(rk[10], rk[13], 3); // rk[7]
                rk[10]     = mm_blend_int64(rk[7], rk[10], 3);
                rk[7]      = t1;
                rk[13]     = t2;

                for (int i = 2; i < 18; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2    = _mm_aesimc_si128(rk[1]);
                rk[1] = mm_blend_int64(t2, rk[1], 3);
            }

            return true;
        }

        static inline __m128i mm_blend_i8(__m128i a, __m128i b, __m128i imm) {
            return _mm_or_si128(_mm_andnot_si128(imm, a), _mm_and_si128(b, imm));
        }

        static inline void rijndael192_encrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK    = _mm_set_epi32(0x00000000, 0x00000000, 0xffff0000, 0xffffff00);
            __m128i PRESHUFFLE_MASK1  = _mm_set_epi32(0x0f0e0d0c, 0x030a0908, 0x0b060504, 0x07020100);
            __m128i POSTSHUFFLE_MASK2 = _mm_set_epi32(0x03068080, 0x80020180, 0x80800504, 0x07808000);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 192 / 8);

            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);

            __m128i key1, key2;
            data1 = _mm_xor_si128(data1, rk[0]);
            data2 = _mm_xor_si128(data2, rk[1]);

            int idx = 1, j;
            for (j = 1; j < r; j++) {
                if (j % 2) {
                    key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                    key2 = mm_blend_shuffle_int64(rk[idx + 1], rk[idx], 3);
                    idx += 2;
                } else {
                    key1 = rk[idx++];
                    key2 = rk[idx];
                }

                data1 = _mm_shuffle_epi8(data1, PRESHUFFLE_MASK1);
                tmp1  = mm_blend_i8(data1, data2, VEC_BLEND_MASK);
                tmp2  = mm_blend_i8(data2, data1, VEC_BLEND_MASK);
                tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
                data1 = _mm_aesenc_si128(tmp1, key1);
                data2 = _mm_aesenc_si128(tmp2, key2);
            }

            if (j % 2) {
                key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                key2 = mm_blend_shuffle_int64(rk[idx + 1], rk[idx], 3);
                idx += 2;
            } else {
                key1 = rk[idx++];
                key2 = rk[idx];
            }
            data1 = _mm_shuffle_epi8(data1, PRESHUFFLE_MASK1);
            tmp1  = mm_blend_i8(data1, data2, VEC_BLEND_MASK);
            tmp2  = mm_blend_i8(data2, data1, VEC_BLEND_MASK);
            tmp2  = _mm_shuffle_epi8(tmp2, POSTSHUFFLE_MASK2);
            tmp1  = _mm_aesenclast_si128(tmp1, key1);
            tmp2  = _mm_aesenclast_si128(tmp2, key2);

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 192 / 8);
            zero_memory(buf, sizeof(buf));
        }

        static inline void rijndael192_decrypt_block(const unsigned char *in, unsigned char *out, int r, __m128i *rk) {
            __m128i tmp1, tmp2, data1, data2;
            __m128i VEC_BLEND_MASK    = _mm_set_epi32(0x00ffff00, 0xffff0000, 0xff000000, 0x00000000);
            __m128i PRESHUFFLE_MASK2  = _mm_set_epi32(0x80060580, 0x03028080, 0x07808004, 0x80800100);
            __m128i POSTSHUFFLE_MASK1 = _mm_set_epi32(0x070e0d0c, 0x0b0a0908, 0x0f060504, 0x03020100);

#ifdef _MSC_VER
            __declspec(align(32))
#else
            __attribute__((aligned(32)))
#endif
                unsigned char buf[32];
            memset(buf, 0, sizeof(buf));
            memcpy(buf, in, 192 / 8);
            data1 = _mm_load_si128(&((__m128i *) buf)[0]);
            data2 = _mm_load_si128(&((__m128i *) buf)[1]);

            __m128i key1, key2;
            data1   = _mm_xor_si128(data1, rk[0]);
            data2   = _mm_xor_si128(data2, rk[1]);
            int idx = 1, j;
            for (j = 1; j < r; j++) {
                if (j % 2) {
                    key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                    key2 = mm_blend_shuffle_int64(rk[idx + 1], rk[idx], 3);
                    idx += 2;
                } else {
                    key1 = rk[idx++];
                    key2 = rk[idx];
                }
                data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
                tmp1  = mm_blend_i8(data1, data2, VEC_BLEND_MASK);
                tmp2  = mm_blend_i8(data2, data1, VEC_BLEND_MASK);
                tmp1  = _mm_shuffle_epi8(tmp1, POSTSHUFFLE_MASK1);
                data1 = _mm_aesdec_si128(tmp1, key1);
                data2 = _mm_aesdec_si128(tmp2, key2);
            }

            if (j % 2) {
                key1 = mm_blend_shuffle_int64(rk[idx], rk[idx + 1], 3);
                key2 = mm_blend_shuffle_int64(rk[idx + 1], rk[idx], 3);
                idx += 2;
            } else {
                key1 = rk[idx++];
                key2 = rk[idx];
            }
            data2 = _mm_shuffle_epi8(data2, PRESHUFFLE_MASK2);
            tmp1  = mm_blend_i8(data1, data2, VEC_BLEND_MASK);
            tmp2  = mm_blend_i8(data2, data1, VEC_BLEND_MASK);
            tmp1  = _mm_shuffle_epi8(tmp1, POSTSHUFFLE_MASK1);
            tmp1  = _mm_aesdeclast_si128(tmp1, key1);
            tmp2  = _mm_aesdeclast_si128(tmp2, key2);

            _mm_store_si128(&((__m128i *) buf)[0], tmp1);
            _mm_store_si128(&((__m128i *) buf)[1], tmp2);
            memcpy(out, buf, 192 / 8);
            zero_memory(buf, sizeof(buf));
        }

        void rijndael192_128_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_encrypt_block(in, out, 12, rk);
        }

        void rijndael192_128_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_decrypt_block(in, out, 12, rk);
        }

        bool rijndael192_192_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            temp2 = _mm_aeskeygenassist_si128(temp3, 0xab);
            KEY_192_ASSIST(&temp1, &temp2, &temp3);
            mm_shuffle_int32(rk[19], temp1, 0, rk[19]);

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[18]);
                std::swap(rk[2], rk[17]);
                std::swap(rk[3], rk[15]);
                std::swap(rk[5], rk[14]);
                std::swap(rk[6], rk[12]);
                std::swap(rk[8], rk[11]);

                __m128i t1 = mm_blend_int64(rk[16], rk[19], 3); // rk[1]
                rk[19]     = rk[1];
                rk[1]      = t1;
                t1         = mm_blend_int64(rk[13], rk[16], 3); // rk[4]
                rk[16]     = mm_blend_int64(rk[19], rk[4], 3);
                __m128i t2 = mm_blend_int64(rk[4], rk[7], 3); // rk[13]
                rk[4]      = t1;
                t1         = mm_blend_int64(rk[10], rk[13], 3); // rk[7]
                rk[10]     = mm_blend_int64(rk[7], rk[10], 3);
                rk[7]      = t1;
                rk[13]     = t2;

                for (int i = 2; i < 18; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2    = _mm_aesimc_si128(rk[1]);
                rk[1] = mm_blend_int64(t2, rk[1], 3);
            }

            zero_memory(keycopy, sizeof(keycopy));
            return true;
        }

        void rijndael192_256_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_encrypt_block(in, out, 14, rk);
        }

        void rijndael192_256_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_decrypt_block(in, out, 14, rk);
        }

        bool rijndael192_256_impl_aesni::init(const unsigned char *key, block_cipher::direction direction) {
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

            temp2 = _mm_aeskeygenassist_si128(temp3, 0x6c);
            KEY_256_ASSIST_1(&temp1, &temp2);
            rk[22] = temp1;

            if (direction == block_cipher::decryption) {
                std::swap(rk[0], rk[21]);
                std::swap(rk[2], rk[20]);
                std::swap(rk[3], rk[18]);
                std::swap(rk[5], rk[17]);
                std::swap(rk[6], rk[15]);
                std::swap(rk[8], rk[14]);
                std::swap(rk[9], rk[12]);

                __m128i t1 = mm_blend_int64(rk[19], rk[22], 3); // rk[1]
                rk[22]     = rk[1];
                rk[1]      = t1;
                t1         = mm_blend_int64(rk[16], rk[19], 3); // rk[4]
                rk[19]     = mm_blend_int64(rk[22], rk[4], 3);
                __m128i t2 = mm_blend_int64(rk[4], rk[7], 3); // rk[16]
                rk[4]      = t1;
                t1         = mm_blend_int64(rk[13], rk[16], 3); // rk[7]
                rk[16]     = t2;
                t2         = mm_blend_int64(rk[7], rk[10], 3); // rk[13]
                rk[10]     = mm_blend_int64(rk[10], rk[13], 3);
                rk[13]     = t2;
                rk[7]      = t1;

                for (int i = 2; i < 21; i++)
                    rk[i] = _mm_aesimc_si128(rk[i]);
                t2    = _mm_aesimc_si128(rk[1]);
                rk[1] = mm_blend_int64(t2, rk[1], 3);
            }

            return true;
        }

        void rijndael192_224_impl_aesni::encrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_encrypt_block(in, out, 13, rk);
        }

        void rijndael192_224_impl_aesni::decrypt_block(const unsigned char *in, unsigned char *out) {
            return rijndael192_decrypt_block(in, out, 13, rk);
        }
    }
}
