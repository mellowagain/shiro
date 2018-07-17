#include <cstdio>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include "pbkdf2.hh"

void PBKDF2_HMAC_SHA_512(const char *pass, int pass_length, const unsigned char *salt, int salt_length, int32_t iterations, uint32_t output_bytes, char *hex_result, uint8_t *bin_result) {
    unsigned int i;
    unsigned char *digest = new unsigned char[output_bytes];
    std::memset(digest, 0, output_bytes);

    PKCS5_PBKDF2_HMAC(pass, pass_length, salt, salt_length, iterations, EVP_sha512(), output_bytes, digest);

    for (i = 0; i < output_bytes; i++) {
        std::sprintf(hex_result + (i * 2), "%02x", 255 & digest[i]);
        bin_result[i] = digest[i];
    }

    delete[] digest;
}
