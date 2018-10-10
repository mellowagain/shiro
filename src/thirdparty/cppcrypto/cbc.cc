/*
This code is written by kerukuro for cppcrypto library (http://cppcrypto.sourceforge.net/)
and released into public domain.
*/

#include "cbc.hh"
#include "cpuinfo.hh"
#include <assert.h>
#include <memory.h>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "portability.hh"

namespace cppcrypto {
    cbc::cbc(const block_cipher &cipher) : block_(0), iv_(0), pos(0), nb_(cipher.blocksize() / 8), cipher_(cipher.clone()) {
        block_ = new unsigned char[nb_];
        iv_    = new unsigned char[nb_];
    }

    cbc::~cbc() {
        zero_memory(block_, nb_);
        zero_memory(iv_, nb_);
        delete[] block_;
        delete[] iv_;
    }

    void cbc::init(const unsigned char *key, size_t keylen, const unsigned char *iv, size_t ivlen, block_cipher::direction direction) {
        assert(keylen == cipher_->keysize() / 8);
        assert(ivlen <= nb_);
        cipher_->init(key, direction);
        memcpy(iv_, iv, ivlen);
        memset(iv_ + ivlen, 0, nb_ - ivlen);
        pos = 0;
    }

    static inline void xor_block_256(const unsigned char *in, const unsigned char *prev, unsigned char *out) {
//#define USE_AVX
#ifdef USE_AVX
        if (cpu_info::avx()) {
            __m256i b1 = _mm256_loadu_si256((const __m256i *) in);
            __m256i p1 = _mm256_loadu_si256((const __m256i *) prev);

            _mm256_storeu_si256((__m256i *) out, _mm256_xor_si256(b1, p1));
            _mm256_zeroupper();
        } else
#endif
            if (cpu_info::sse2()) {
            __m128i b1 = _mm_loadu_si128((const __m128i *) in);
            __m128i p1 = _mm_loadu_si128((const __m128i *) prev);
            __m128i b2 = _mm_loadu_si128((const __m128i *) (in + 16));
            __m128i p2 = _mm_loadu_si128((const __m128i *) (prev + 16));

            _mm_storeu_si128((__m128i *) out, _mm_xor_si128(b1, p1));
            _mm_storeu_si128((__m128i *) (out + 16), _mm_xor_si128(b2, p2));
        } else {
            for (int i = 0; i < 32; i++)
                out[i] = in[i] ^ prev[i];
        }
    }

    static inline void xor_block_128(const unsigned char *in, const unsigned char *prev, unsigned char *out) {
        if (cpu_info::sse2()) {
            __m128i b = _mm_loadu_si128((const __m128i *) in);
            __m128i p = _mm_loadu_si128((const __m128i *) prev);

            _mm_storeu_si128((__m128i *) out, _mm_xor_si128(b, p));
        } else {
            for (int i = 0; i < 16; i++)
                out[i] = in[i] ^ prev[i];
        }
    }

    static inline void xor_block_128n(const unsigned char *in, const unsigned char *prev, unsigned char *out, size_t n) {
        if (cpu_info::sse2()) {
            __m128i b = _mm_loadu_si128((const __m128i *) in);
            __m128i p = _mm_loadu_si128((const __m128i *) prev);

            _mm_storeu_si128((__m128i *) out, _mm_xor_si128(b, p));
            for (size_t i = 16; i < n; i++)
                out[i] = in[i] ^ prev[i];
        } else {
            for (size_t i = 0; i < n; i++)
                out[i] = in[i] ^ prev[i];
        }
    }

    static inline void xor_block_512(const unsigned char *in, const unsigned char *prev, unsigned char *out) {
#ifdef USE_AVX
        if (cpu_info::avx()) {
            __m256i b1 = _mm256_loadu_si256((const __m256i *) in);
            __m256i p1 = _mm256_loadu_si256((const __m256i *) prev);
            __m256i b2 = _mm256_loadu_si256((const __m256i *) (in + 32));
            __m256i p2 = _mm256_loadu_si256((const __m256i *) (prev + 32));

            _mm256_storeu_si256((__m256i *) out, _mm256_xor_si256(b1, p1));
            _mm256_storeu_si256((__m256i *) (out + 32), _mm256_xor_si256(b2, p2));
            _mm256_zeroupper();
        } else
#endif
            if (cpu_info::sse2()) {
            __m128i b1 = _mm_loadu_si128((const __m128i *) in);
            __m128i p1 = _mm_loadu_si128((const __m128i *) prev);
            __m128i b2 = _mm_loadu_si128((const __m128i *) (in + 16));
            __m128i p2 = _mm_loadu_si128((const __m128i *) (prev + 16));

            _mm_storeu_si128((__m128i *) out, _mm_xor_si128(b1, p1));
            _mm_storeu_si128((__m128i *) (out + 16), _mm_xor_si128(b2, p2));

            b1 = _mm_loadu_si128((const __m128i *) (in + 32));
            p1 = _mm_loadu_si128((const __m128i *) (prev + 32));
            b2 = _mm_loadu_si128((const __m128i *) (in + 48));
            p2 = _mm_loadu_si128((const __m128i *) (prev + 48));

            _mm_storeu_si128((__m128i *) (out + 32), _mm_xor_si128(b1, p1));
            _mm_storeu_si128((__m128i *) (out + 48), _mm_xor_si128(b2, p2));

        } else {
            for (int i = 0; i < 64; i++)
                out[i] = in[i] ^ prev[i];
        }
    }

    void cbc::encrypt_update(const unsigned char *in, size_t len, unsigned char *out, size_t &resultlen) {
        resultlen = 0;
        size_t nb = nb_;
        if (pos && pos + len >= nb) {
            memcpy(block_ + pos, in, nb - pos);
            for (unsigned int i = 0; i < nb; i++)
                block_[i] ^= iv_[i];
            cipher_->encrypt_block(block_, out);
            memcpy(iv_, out, nb);
            in += nb - pos;
            len -= nb - pos;
            out += nb;
            resultlen += nb;
            pos = 0;
        }
        if (len >= nb) {
            if (nb == 16) // optimization for the most common block sizes
            {
                size_t               blocks = len / 16;
                size_t               bytes  = blocks * 16;
                const unsigned char *prev   = iv_;

                for (size_t i = 0; i < blocks; i++) {
                    xor_block_128(in, prev, block_);
                    cipher_->encrypt_block(block_, out);
                    prev = out;
                    in += 16;
                    out += 16;
                }
                memcpy(iv_, out - 16, 16);
                resultlen += bytes;
                len -= bytes;
            } else if (nb == 32) // optimization for the most common block sizes
            {
                size_t               blocks = len / 32;
                size_t               bytes  = blocks * 32;
                const unsigned char *prev   = iv_;
                for (size_t i = 0; i < blocks; i++) {
                    xor_block_256(in, prev, block_);
                    cipher_->encrypt_block(block_, out);
                    prev = out;
                    in += 32;
                    out += 32;
                }
                memcpy(iv_, out - 32, 32);
                resultlen += bytes;
                len -= bytes;
            } else if (nb == 64) // optimization for the most common block sizes
            {
                size_t               blocks = len / 64;
                size_t               bytes  = blocks * 64;
                const unsigned char *prev   = iv_;
                for (size_t i = 0; i < blocks; i++) {
                    xor_block_512(in, prev, block_);
                    cipher_->encrypt_block(block_, out);
                    prev = out;
                    in += 64;
                    out += 64;
                }
                memcpy(iv_, out - 64, 64);
                resultlen += bytes;
                len -= bytes;
            } else if (nb > 16 && nb < 32) {
                size_t               blocks = len / nb;
                size_t               bytes  = blocks * nb;
                const unsigned char *prev   = iv_;

                for (size_t i = 0; i < blocks; i++) {
                    xor_block_128n(in, prev, block_, nb);
                    cipher_->encrypt_block(block_, out);
                    prev = out;
                    in += nb;
                    out += nb;
                }
                memcpy(iv_, out - nb, nb);
                resultlen += bytes;
                len -= bytes;
            } else {
                size_t blocks = len / nb;
                size_t bytes  = blocks * nb;
                for (size_t i = 0; i < blocks; i++) {
                    for (unsigned int i = 0; i < nb; i++)
                        block_[i] = in[i] ^ iv_[i];
                    cipher_->encrypt_block(block_, out);
                    memcpy(iv_, out, nb);
                    in += nb;
                    out += nb;
                }
                resultlen += bytes;
                len -= bytes;
            }
        }
        // TODO possible xor with iv directly here and in the first if instead of memcpy
        memcpy(block_ + pos, in, len);
        pos += len;
    }

    void cbc::encrypt_final(unsigned char *out, size_t &resultlen) {
        int padding = static_cast<int>(nb_) - static_cast<int>(pos);
        memset(block_ + pos, padding, padding);
        for (size_t i = 0; i < nb_; i++)
            block_[i] ^= iv_[i];
        cipher_->encrypt_block(block_, out);
        resultlen = nb_;
    }

    void cbc::decrypt_update(const unsigned char *in, size_t len, unsigned char *out, size_t &resultlen) {
        resultlen = 0;
        size_t nb = nb_;
        if (pos && pos + len > nb) {
            memcpy(block_ + pos, in, nb - pos);
            cipher_->decrypt_block(block_, out);
            for (size_t i = 0; i < nb; i++)
                out[i] ^= iv_[i];
            memcpy(iv_, block_, nb);
            in += nb - pos;
            len -= nb - pos;
            out += nb;
            resultlen += nb;
            pos = 0;
        }
        if (len > nb) {
            if (nb == 16) // optimization for the most common block sizes
            {
                size_t               blocks = (len - 1) / 16;
                size_t               bytes  = blocks * 16;
                const unsigned char *prev   = iv_;

                for (size_t i = 0; i < blocks; i++) {
                    cipher_->decrypt_block(in, block_);
                    xor_block_128(block_, prev, out);

                    prev = in;
                    in += 16;
                    out += 16;
                }
                memcpy(iv_, in - 16, 16);
                resultlen += bytes;
                len -= bytes;
            } else if (nb == 32) // optimization for the most common block sizes
            {
                size_t               blocks = (len - 1) / 32;
                size_t               bytes  = blocks * 32;
                const unsigned char *prev   = iv_;
                for (size_t i = 0; i < blocks; i++) {
                    cipher_->decrypt_block(in, block_);
                    xor_block_256(block_, prev, out);
                    prev = in;
                    in += 32;
                    out += 32;
                }
                memcpy(iv_, in - 32, 32);
                resultlen += bytes;
                len -= bytes;
            } else if (nb == 64) // optimization for the most common block sizes
            {
                size_t               blocks = (len - 1) / 64;
                size_t               bytes  = blocks * 64;
                const unsigned char *prev   = iv_;
                for (size_t i = 0; i < blocks; i++) {
                    cipher_->decrypt_block(in, block_);
                    xor_block_512(block_, prev, out);
                    prev = in;
                    in += 64;
                    out += 64;
                }
                memcpy(iv_, in - 64, 64);
                resultlen += bytes;
                len -= bytes;
            } else if (nb > 16 && nb < 32) // optimization for the most common block sizes
            {
                size_t               blocks = (len - 1) / nb;
                size_t               bytes  = blocks * nb;
                const unsigned char *prev   = iv_;

                for (size_t i = 0; i < blocks; i++) {
                    cipher_->decrypt_block(in, block_);
                    xor_block_128n(block_, prev, out, nb);

                    prev = in;
                    in += nb;
                    out += nb;
                }
                memcpy(iv_, in - nb, nb);
                resultlen += bytes;
                len -= bytes;
            } else {
                size_t blocks = (len - 1) / nb;
                size_t bytes  = blocks * nb;
                for (size_t i = 0; i < blocks; i++) {
                    cipher_->decrypt_block(in, block_);
                    for (unsigned int i = 0; i < nb; i++)
                        out[i] = block_[i] ^ iv_[i];
                    memcpy(iv_, in, nb);
                    in += nb;
                    out += nb;
                }
                resultlen += bytes;
                len -= bytes;
            }
        }
        // TODO possible xor with iv directly here and in the first if instead of memcpy
        memcpy(block_ + pos, in, len);
        pos += len;
    }

    void cbc::decrypt_final(unsigned char *out, size_t &resultlen) {
        assert(pos == nb_);
        cipher_->decrypt_block(block_, block_);
        for (size_t i = 0; i < nb_; i++)
            block_[i] ^= iv_[i];
        unsigned char padding = block_[pos - 1];
        if (padding <= nb_) {
            resultlen = nb_ - padding;
            memcpy(out, block_, resultlen);
        } else
            resultlen = 0;
    }

    void cbc::encrypt_update(const unsigned char *in, size_t len, std::ostream &out) {
        unsigned char buf[1024];
        size_t        maxsize = sizeof(buf) - nb_;
        for (size_t i = 0; i < len; i += maxsize) {
            size_t resultlen;
            encrypt_update(in + i, std::min(maxsize, len - i), buf, resultlen);
            if (resultlen)
                out.write(reinterpret_cast<char *>(buf), resultlen);
        }
    }

    void cbc::encrypt_final(std::ostream &out) {
        unsigned char buf[1024];
        size_t        resultlen;
        encrypt_final(buf, resultlen);
        if (resultlen)
            out.write(reinterpret_cast<char *>(buf), resultlen);
    }

    void cbc::decrypt_update(const unsigned char *in, size_t len, std::ostream &out) {
        unsigned char buf[1024];
        size_t        maxsize = sizeof(buf) - nb_;
        for (size_t i = 0; i < len; i += maxsize) {
            size_t resultlen;
            decrypt_update(in + i, std::min(maxsize, len - i), buf, resultlen);
            if (resultlen)
                out.write(reinterpret_cast<char *>(buf), resultlen);
        }
    }

    void cbc::decrypt_final(std::ostream &out) {
        unsigned char buf[1024];
        size_t        resultlen;
        decrypt_final(buf, resultlen);
        if (resultlen)
            out.write(reinterpret_cast<char *>(buf), resultlen);
    }

    void cbc::encrypt_update(const unsigned char *in, size_t len, std::vector<unsigned char> &out) {
        auto oldsize = out.size();
        out.resize(oldsize + ((pos + len) / nb_) * nb_);
        size_t resultlen;
        encrypt_update(in, len, &out[oldsize], resultlen);
        assert(resultlen == out.size() - oldsize);
    }

    void cbc::encrypt_final(std::vector<unsigned char> &out) {
        auto oldsize = out.size();
        out.resize(oldsize + nb_);
        size_t resultlen;
        encrypt_final(&out[oldsize], resultlen);
        assert(resultlen == out.size() - oldsize);
    }

    void cbc::decrypt_update(const unsigned char *in, size_t len, std::vector<unsigned char> &out) {
        auto oldsize = out.size();
        out.resize(oldsize + ((pos + len - 1) / nb_) * nb_);
        size_t resultlen;
        decrypt_update(in, len, &out[oldsize], resultlen);
        assert(resultlen == out.size() - oldsize);
    }

    void cbc::decrypt_final(std::vector<unsigned char> &out) {
        auto oldsize = out.size();
        out.resize(oldsize + nb_);
        size_t resultlen;
        decrypt_final(&out[oldsize], resultlen);
        out.resize(oldsize + resultlen);
    }
}
