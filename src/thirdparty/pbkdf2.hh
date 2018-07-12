#ifndef SHIRO_PBKDF2_HH
#define SHIRO_PBKDF2_HH

#include <cstdint>

// Wrapper around OpenSSL PKBDF2

void PBKDF2_HMAC_SHA_512(const char *pass, int pass_length, const unsigned char *salt, int salt_length, int32_t iterations, uint32_t output_bytes, char *hex_result, uint8_t *bin_result);

#endif  //SHIRO_PBKDF2_HH
