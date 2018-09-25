/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <lzma.h>
#include <mcrypt.h>
#include <memory>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

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
    MCRYPT td = mcrypt_module_open((char*) MCRYPT_RIJNDAEL_256, nullptr, (char*) MCRYPT_CBC, nullptr);

    if (td == MCRYPT_FAILED)
        return {};

    std::vector<char> key_vector(key.c_str(), key.c_str() + key.size() + 1);
    std::vector<unsigned char> result(cipher);

    mcrypt_generic_init(td, &key_vector[0], key_vector.size() - 1, &iv[0]);

    mdecrypt_generic(td, &result[0], result.size());

    mcrypt_generic_deinit(td);
    mcrypt_module_close(td);

    return result;
}

std::string shiro::utils::crypto::pbkdf2_hmac_sha512::hash(std::string input, std::string salt) {
    unsigned char result[64];
    char output[128];

    PKCS5_PBKDF2_HMAC(input.c_str(), input.length(), reinterpret_cast<const unsigned char*>(salt.c_str()), salt.size(), 4096, EVP_sha512(), 64, result);

    for (size_t i = 0; i < 64; i++) {
        std::snprintf(&output[i * 2], sizeof(output), "%02x", (unsigned int) result[i]);
    }

    return std::string(output);
}

std::string shiro::utils::crypto::md5::hash(const std::string &input) {
    unsigned char result[16];
    char output[33];

    MD5((unsigned char*) input.c_str(), input.length(), result);

    for (size_t i = 0; i < 16; i++) {
        std::snprintf(&output[i * 2], sizeof(output), "%02x", (unsigned int) result[i]);
    }

    return std::string(output);
}

std::string shiro::utils::crypto::lzma::decompress(std::string input) {
    lzma_stream stream = LZMA_STREAM_INIT;

    lzma_ret decoder = lzma_stream_decoder(&stream, UINT64_MAX, LZMA_CONCATENATED);

    if (decoder != LZMA_OK)
        return "";

    std::string output;
    output.resize(BUFSIZ);

    size_t amount = 0;
    size_t available = output.size();

    stream.next_in = reinterpret_cast<const uint8_t*>(input.data());
    stream.avail_in = input.size();
    stream.next_out = reinterpret_cast<uint8_t*>(&output[0]);
    stream.avail_out = available;

    while (true) {
        decoder = lzma_code(&stream, stream.avail_in == 0 ? LZMA_FINISH : LZMA_RUN);
        if (decoder == LZMA_STREAM_END) {
            amount += available - stream.avail_in;

            if (stream.avail_in == 0)
                break;

            output.resize(amount);
            lzma_end(&stream);
            return output;
        }

        if (decoder != LZMA_OK)
            break;

        if (stream.avail_out == 0) {
            amount += available - stream.avail_out;
            output.resize(input.size() << 1);
            stream.next_out = reinterpret_cast<uint8_t*>(&output[0] + amount);
            stream.avail_out = available = output.size() - amount;
        }
    }

    return "";
}
