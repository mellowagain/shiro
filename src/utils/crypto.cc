#include <cstring>
#include <mcrypt.h>
#include <memory>
#include <openssl/evp.h>

#include "crypto.hh"

std::vector<unsigned char> shiro::utils::crypto::base64::decode(const char *base64) {
    const size_t max_len = std::strlen(base64) / 4 * 3 + 1;

    std::unique_ptr<BIO, bio_free_all> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);

    BIO *source = BIO_new_mem_buf(base64, -1);
    BIO_push(b64.get(), source);

    std::vector<unsigned char> result(max_len);

    const int length = BIO_read(b64.get(), result.data(), (int) max_len);
    result.resize((unsigned long) length);

    return result;
}

std::vector<unsigned char> shiro::utils::crypto::rijndael256::decode(std::vector<unsigned char> iv, std::string key, std::vector<unsigned char> cipher) {
    static char algorithm[] = "rijndael-256";
    static char mode[] = "cbc";

    std::vector<char> key_vector(key.c_str(), key.c_str() + key.size() + 1);

    MCRYPT td = mcrypt_module_open(algorithm, nullptr, mode, nullptr);

    if (td == MCRYPT_FAILED)
        return {};

    std::vector<unsigned char> result(cipher);

    mcrypt_generic_init(td, &key_vector[0], key_vector.size() - 1, &iv[0]);

    mdecrypt_generic(td, &result[0], result.size());

    mcrypt_generic_deinit(td);
    mcrypt_module_close(td);

    return result;
}
