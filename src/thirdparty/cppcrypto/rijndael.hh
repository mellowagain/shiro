/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#ifndef CPPCRYPTO_RIJNDAEL_H
#define CPPCRYPTO_RIJNDAEL_H

#include <stdint.h>
#include "alignedarray.hh"
#include "block_cipher.hh"
#include "rijndael-impl.hh"

namespace cppcrypto {

    class rijndael128_128 : public block_cipher {
    public:
        rijndael128_128();
        ~rijndael128_128();

        size_t blocksize() const override {
            return 128;
        }
        size_t keysize() const override {
            return 128;
        }
        rijndael128_128 *clone() const override {
            return new rijndael128_128;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 44, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael128_160 : public block_cipher {
    public:
        rijndael128_160();
        ~rijndael128_160();

        size_t blocksize() const override {
            return 128;
        }
        size_t keysize() const override {
            return 160;
        }
        rijndael128_160 *clone() const override {
            return new rijndael128_160;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 48, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael128_192 : public block_cipher {
    public:
        rijndael128_192();
        ~rijndael128_192();

        size_t blocksize() const override {
            return 128;
        }
        size_t keysize() const override {
            return 192;
        }
        rijndael128_192 *clone() const override {
            return new rijndael128_192;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 52, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael128_224 : public block_cipher {
    public:
        rijndael128_224();
        ~rijndael128_224();

        size_t blocksize() const override {
            return 128;
        }
        size_t keysize() const override {
            return 224;
        }
        rijndael128_224 *clone() const override {
            return new rijndael128_224;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 56, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael128_256 : public block_cipher {
    public:
        rijndael128_256();
        ~rijndael128_256();

        size_t blocksize() const override {
            return 128;
        }
        size_t keysize() const override {
            return 256;
        }
        rijndael128_256 *clone() const override {
            return new rijndael128_256;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 60, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    namespace detail {
        class rijndael256 : public block_cipher {
        public:
            rijndael256();
            ~rijndael256();
            void clear() override;

            size_t blocksize() const override {
                return 256;
            }

            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;

        protected:
            aligned_pod_array<uint32_t, 120, 64> W_;
            detail::rijndael_impl *              impl_;
        };
    }

    class rijndael256_128 : public detail::rijndael256 {
    public:
        rijndael256_128();

        size_t keysize() const override {
            return 128;
        }
        rijndael256_128 *clone() const override {
            return new rijndael256_128;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael256_160 : public detail::rijndael256 {
    public:
        rijndael256_160();

        size_t keysize() const override {
            return 160;
        }
        rijndael256_160 *clone() const override {
            return new rijndael256_160;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael256_192 : public detail::rijndael256 {
    public:
        rijndael256_192();

        size_t keysize() const override {
            return 192;
        }
        rijndael256_192 *clone() const override {
            return new rijndael256_192;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael256_224 : public detail::rijndael256 {
    public:
        rijndael256_224();

        size_t keysize() const override {
            return 224;
        }
        rijndael256_224 *clone() const override {
            return new rijndael256_224;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael256_256 : public detail::rijndael256 {
    public:
        rijndael256_256();

        size_t keysize() const override {
            return 256;
        }
        rijndael256_256 *clone() const override {
            return new rijndael256_256;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael192_128 : public block_cipher {
    public:
        rijndael192_128();
        ~rijndael192_128();

        size_t blocksize() const override {
            return 192;
        }
        size_t keysize() const override {
            return 128;
        }
        rijndael192_128 *clone() const override {
            return new rijndael192_128;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 78, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael192_160 : public block_cipher {
    public:
        rijndael192_160();
        ~rijndael192_160();

        size_t blocksize() const override {
            return 192;
        }
        size_t keysize() const override {
            return 160;
        }
        rijndael192_160 *clone() const override {
            return new rijndael192_160;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 78, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael192_192 : public block_cipher {
    public:
        rijndael192_192();
        ~rijndael192_192();

        size_t blocksize() const override {
            return 192;
        }
        size_t keysize() const override {
            return 192;
        }
        rijndael192_192 *clone() const override {
            return new rijndael192_192;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 78, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael192_224 : public block_cipher {
    public:
        rijndael192_224();
        ~rijndael192_224();

        size_t blocksize() const override {
            return 192;
        }
        size_t keysize() const override {
            return 224;
        }
        rijndael192_224 *clone() const override {
            return new rijndael192_224;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 84, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael192_256 : public block_cipher {
    public:
        rijndael192_256();
        ~rijndael192_256();

        size_t blocksize() const override {
            return 192;
        }
        size_t keysize() const override {
            return 256;
        }
        rijndael192_256 *clone() const override {
            return new rijndael192_256;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 90, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael160_128 : public block_cipher {
    public:
        rijndael160_128();
        ~rijndael160_128();

        size_t blocksize() const override {
            return 160;
        }
        size_t keysize() const override {
            return 128;
        }
        rijndael160_128 *clone() const override {
            return new rijndael160_128;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 60, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael160_160 : public block_cipher {
    public:
        rijndael160_160();
        ~rijndael160_160();

        size_t blocksize() const override {
            return 160;
        }
        size_t keysize() const override {
            return 160;
        }
        rijndael160_160 *clone() const override {
            return new rijndael160_160;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 60, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael160_192 : public block_cipher {
    public:
        rijndael160_192();
        ~rijndael160_192();

        size_t blocksize() const override {
            return 160;
        }
        size_t keysize() const override {
            return 192;
        }
        rijndael160_192 *clone() const override {
            return new rijndael160_192;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 65, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael160_224 : public block_cipher {
    public:
        rijndael160_224();
        ~rijndael160_224();

        size_t blocksize() const override {
            return 160;
        }
        size_t keysize() const override {
            return 224;
        }
        rijndael160_224 *clone() const override {
            return new rijndael160_224;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 70, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    class rijndael160_256 : public block_cipher {
    public:
        rijndael160_256();
        ~rijndael160_256();

        size_t blocksize() const override {
            return 160;
        }
        size_t keysize() const override {
            return 256;
        }
        rijndael160_256 *clone() const override {
            return new rijndael160_256;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 75, 64> W_;
        detail::rijndael_impl *             impl_;
    };

    namespace detail {
        class rijndael224 : public block_cipher {
        public:
            rijndael224();
            ~rijndael224();
            void clear() override;

            size_t blocksize() const override {
                return 224;
            }

            void encrypt_block(const unsigned char *in, unsigned char *out) override;
            void decrypt_block(const unsigned char *in, unsigned char *out) override;

        protected:
            aligned_pod_array<uint32_t, 98, 64> W_;
            detail::rijndael_impl *             impl_;
        };
    }

    class rijndael224_128 : public detail::rijndael224 {
    public:
        rijndael224_128();
        size_t keysize() const override {
            return 128;
        }
        rijndael224_128 *clone() const override {
            return new rijndael224_128;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael224_160 : public detail::rijndael224 {
    public:
        rijndael224_160();
        size_t keysize() const override {
            return 160;
        }
        rijndael224_160 *clone() const override {
            return new rijndael224_160;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael224_192 : public detail::rijndael224 {
    public:
        rijndael224_192();
        size_t keysize() const override {
            return 192;
        }
        rijndael224_192 *clone() const override {
            return new rijndael224_192;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael224_224 : public detail::rijndael224 {
    public:
        rijndael224_224();
        size_t keysize() const override {
            return 224;
        }
        rijndael224_224 *clone() const override {
            return new rijndael224_224;
        }

        bool init(const unsigned char *key, block_cipher::direction direction) override;
    };

    class rijndael224_256 : public block_cipher {
    public:
        rijndael224_256();
        ~rijndael224_256();

        size_t blocksize() const override {
            return 224;
        }
        size_t keysize() const override {
            return 256;
        }
        rijndael224_256 *clone() const override {
            return new rijndael224_256;
        }
        void clear() override;

        bool init(const unsigned char *key, block_cipher::direction direction) override;
        void encrypt_block(const unsigned char *in, unsigned char *out) override;
        void decrypt_block(const unsigned char *in, unsigned char *out) override;

    private:
        aligned_pod_array<uint32_t, 105, 64> W_;
        detail::rijndael_impl *              impl_;
    };
}

#endif
