#ifndef SHIRO_CRYPTO_HH
#define SHIRO_CRYPTO_HH

#include <openssl/bio.h>
#include <string>
#include <vector>

namespace shiro::utils::crypto {

    namespace base64 {
        std::vector<unsigned char> decode(const char *base64);
    }

    namespace rijndael256 {
        std::vector<unsigned char> decode(std::vector<unsigned char> iv, std::string key, std::vector<unsigned char> cipher);
    }

    namespace pbkdf2_hmac_sha512 {
        std::string hash(std::string input, std::string salt);
    }

    namespace md5 {
        std::string hash(const std::string &input);
    }

    namespace lzma {
        std::string decompress(std::string input);
    }

    // Anonymous utility namespace
    namespace {
        struct bio_free_all {
            void operator()(BIO *p) {
                BIO_free_all(p);
            }
        };
    }

}

#endif //SHIRO_CRYPTO_HH
