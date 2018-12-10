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
#include <memory>
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <ctime>

#include "../thirdparty/cppcrypto/cbc.hh"
#include "../thirdparty/cppcrypto/rijndael.hh"
#include "crypto.hh"
#include "../native/process_info.hh"

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
    using namespace cppcrypto;

    std::unique_ptr<cbc> decryptor = std::make_unique<cbc>(rijndael256_256());
    decryptor->init((unsigned char*) key.c_str(), key.size(), &iv[0], iv.size(), block_cipher::direction::decryption);

    std::vector<unsigned char> result;
    decryptor->decrypt_update(&cipher[0], cipher.size(), result);
    decryptor->decrypt_final(result);

    return result;
}

std::string shiro::utils::crypto::pbkdf2_hmac_sha512::hash(std::string input, std::string salt) {
    unsigned char result[64];
    char output[256];

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

uint32_t shiro::utils::crypto::get_highest_bit(uint32_t bitwise) {
    if (bitwise == 0)
        return 0;

    uint32_t result = 1;

    while (bitwise >>= 1) {
        result <<= 1;
    }

    return result;
}

uint32_t shiro::utils::crypto::make_seed() {
    int32_t a = (int32_t) std::clock();
    int32_t b = (int32_t) std::time(nullptr);
    int32_t c = native::process_info::get_pid();

    a = a - b;
    a = a - c;
    a = a ^ (c >> 13);

    b = b - c;
    b = b - a;
    b = b ^ (a << 8);

    c = c - a;
    c = c - b;
    c = c ^ (b >> 13);

    a = a - b;
    a = a - c;
    a = a ^ (c >> 12);

    b = b - c;
    b = b - a;
    b = b ^ (a << 16);

    c = c - a;
    c = c - b;
    c = c ^ (b >> 5);

    a = a - b;
    a = a - c;
    a = a ^ (c >> 3);

    b = b - c;
    b = b - a;
    b = b ^ (a << 10);

    c = c - a;
    c = c - b;
    c = c ^ (b >> 15);

    return (uint32_t) c;
}
