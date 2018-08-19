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
