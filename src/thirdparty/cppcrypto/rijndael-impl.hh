/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#ifndef CPPCRYPTO_RIJNDAELIMPL_H
#define CPPCRYPTO_RIJNDAELIMPL_H

#include <stdint.h>
#include <emmintrin.h>
#include "block_cipher.hh"

namespace cppcrypto {
    namespace detail {
        class rijndael_impl {
        public:
            virtual ~rijndael_impl() {
            }
            virtual bool init(const unsigned char *key, block_cipher::direction direction) = 0;
            virtual void encrypt_block(const unsigned char *in, unsigned char *out)        = 0;
            virtual void decrypt_block(const unsigned char *in, unsigned char *out)        = 0;
        };

        class rijndael128_128_impl_aesni : public rijndael_impl {
        private:
            __m128i rk[11];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael128_160_impl_aesni : public rijndael_impl {
        private:
            __m128i rk[12];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael128_192_impl_aesni : public rijndael_impl {
        private:
            __m128i rk[13];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael128_224_impl_aesni : public rijndael_impl {
        private:
            __m128i rk[14];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael128_256_impl_aesni : public rijndael_impl {
        private:
            __m128i rk[15];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael256_256_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[30];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael256_128_impl_aesni : public rijndael256_256_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael256_224_impl_aesni : public rijndael256_256_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael256_160_impl_aesni : public rijndael256_256_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael256_192_impl_aesni : public rijndael256_256_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael192_128_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[20];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael192_160_impl_aesni : public rijndael192_128_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael192_192_impl_aesni : public rijndael192_128_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael192_224_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[21];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael192_256_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[23];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael160_128_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[15];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael160_160_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[15];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael160_192_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[17];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael160_224_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[18];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael160_256_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[19];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael224_128_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[25];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };

        class rijndael224_160_impl_aesni : public rijndael224_128_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael224_192_impl_aesni : public rijndael224_128_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael224_224_impl_aesni : public rijndael224_128_impl_aesni {
        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
        };

        class rijndael224_256_impl_aesni : public rijndael_impl {
        protected:
            __m128i rk[27];

        public:
            bool init(const unsigned char *key, block_cipher::direction direction) override;
            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;
        };
    }
}
#endif
