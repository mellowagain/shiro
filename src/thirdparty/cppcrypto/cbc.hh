/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#ifndef CPPCRYPTO_CBC_H
#define CPPCRYPTO_CBC_H

#include <memory>
#include <ostream>
#include <stdint.h>
#include <vector>

#include "block_cipher.hh"

namespace cppcrypto {
    class cbc {
    public:
        cbc(const block_cipher &cipher);
        virtual ~cbc();

        void init(const unsigned char *key, size_t keylen, const unsigned char *iv, size_t ivlen, block_cipher::direction direction);

        void encrypt_update(const unsigned char *in, size_t len, unsigned char *out, size_t &resultlen);
        void encrypt_final(unsigned char *out, size_t &resultlen);
        void decrypt_update(const unsigned char *in, size_t len, unsigned char *out, size_t &resultlen);
        void decrypt_final(unsigned char *out, size_t &resultlen);

        // These slower variants append the output to the vector, dynamically resizing the vector as needed
        void encrypt_update(const unsigned char *in, size_t len, std::vector<unsigned char> &out);
        void encrypt_final(std::vector<unsigned char> &out);
        void decrypt_update(const unsigned char *in, size_t len, std::vector<unsigned char> &out);
        void decrypt_final(std::vector<unsigned char> &out);

        // These slower variants write the output to std::ostream
        void encrypt_update(const unsigned char *in, size_t len, std::ostream &out);
        void encrypt_final(std::ostream &out);
        void decrypt_update(const unsigned char *in, size_t len, std::ostream &out);
        void decrypt_final(std::ostream &out);

        size_t keysize() const {
            return cipher_->keysize();
        }
        size_t ivsize() const {
            return cipher_->blocksize();
        }

    private:
        cbc(const cbc &) = delete;
        void operator=(const cbc &) = delete;

        unsigned char *               block_;
        unsigned char *               iv_;
        size_t                        pos;
        size_t                        nb_;
        std::unique_ptr<block_cipher> cipher_;
    };
}

#endif
