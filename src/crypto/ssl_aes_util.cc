#include "crypto/ssl_aes_util.h"
#include "crypto/openssl_util.h"

#include "third_party/boringssl/include/openssl/aes.h"
#include "third_party/boringssl/include/openssl/evp.h"

#include "base/location.h"
#include "strings/string_util.h"

#include "io/input_stream.h"
#include "io/output_stream.h"
#include "io/io_util.h"

#include <memory>
#include <string>

#include <glog/logging.h>

using io::InputStream;
using io::OutputStream;

namespace crypto {

namespace {

enum Cipher {
  kEncrypt,
  kDecrypt,
};

const EVP_CIPHER* CBCGetCipherForKey(const std::string& key) {
  switch (key.length()) {
    case 16: return EVP_aes_128_cbc();
    case 32: return EVP_aes_256_cbc();
    default:
      return nullptr;
  }
}

const EVP_CIPHER* ECBGetCipherForKey(const std::string& key) {
  switch (key.length()) {
    case 16: return EVP_aes_128_ecb();
    case 32: return EVP_aes_256_ecb();
    default: return nullptr;
  }
}

class ScopedCipherCTX {
 public:
  explicit ScopedCipherCTX() {
    EVP_CIPHER_CTX_init(&ctx_);
  }
  ~ScopedCipherCTX() {
    EVP_CIPHER_CTX_cleanup(&ctx_);
    crypto::ClearOpenSSLERRStack(FROM_HERE);
  }
  EVP_CIPHER_CTX* get() { return &ctx_; }

 private:
  EVP_CIPHER_CTX ctx_;
};

} // namespace

// CBC ECB
bool CryptInternalNoCounter(Cipher cip,
                            const std::string& raw_key,
                            const EVP_CIPHER* cipher,
                            io::InputStream* in,
                            io::OutputStream* out,
                            const std::string& iv="") {
  if (raw_key.empty()) {
    LOG(ERROR) << "AES Key Empty";
    return false;
  }
  if (!cipher) {
    LOG(ERROR) << "EVP_CIPHER Empty";
    return false;
  }
  bool do_cbc = false;
  bool do_encrypt = cip == kEncrypt ? true : false;

  if (!iv.empty() && iv.size() == 16) { // CBC
    do_cbc = true;
    if (EVP_CIPHER_iv_length(cipher) != iv.length()) {
      LOG(ERROR) << "EVP_CIPHER_iv_length != " << iv.length();
      return false;
    } 
  } else {
    do_cbc = false;
  }


  ScopedCipherCTX ctx;
  if (!EVP_CipherInit_ex(ctx.get(), cipher, nullptr,
                         reinterpret_cast<const uint8_t*>(raw_key.data()),
                do_cbc ? reinterpret_cast<const uint8_t*>(iv.data()) : nullptr,
                         do_encrypt)) {
    LOG(ERROR) << "EVP_CipherInit_ex: ERROR";
    return false;
  }

  int last_block = false;
  while (true) {
    char in_buffer[AES_BLOCK_SIZE] = {0};
    char out_buffer[AES_BLOCK_SIZE * 2] = {0};
    int in_buffer_len = 0;

    in_buffer_len = AES_BLOCK_SIZE;

    int ret = io::IOUtil::ReadFromInput(in, (void*)in_buffer, in_buffer_len);
    if (ret == 0) { // No input data
      return true;
    } else if (ret < in_buffer_len) {
      last_block = true;
    } else if (ret == in_buffer_len) {
      if (!io::IOUtil::PeekInput(in)) {
        last_block = true;
      } 
    }

    uint8_t* out_ptr = reinterpret_cast<uint8_t*>(out_buffer);
    int out_len;
    if (!EVP_CipherUpdate(ctx.get(),
                          out_ptr,
                          &out_len,
                          (const uint8_t*)in_buffer,
                          (size_t)ret)) {
      LOG(ERROR) << "CipherUpdate: in_buffer_len: " << ret << ", bytes ERROR";
      return false;
    }

    int tail_len = 0;
    if (last_block) {
      // padding
      if (!EVP_CipherFinal_ex(ctx.get(), out_ptr + out_len, &tail_len)) {
        LOG(ERROR) << "EVP_CipherFinal_ex: ERROR";
        return false;
      }
    }
    
    if (!io::IOUtil::WriteToOutput(out, out_ptr, out_len + tail_len)) {
      return false;
    }

    if (last_block) { // the last block
      return true;
    }
  }
  return true;
}

bool SslAESUtil::CBCEncrypt(const std::string& raw_key,
                            const std::string& iv,
                            io::InputStream* in,
                            io::OutputStream* out) {
  return CryptInternalNoCounter(kEncrypt, raw_key, CBCGetCipherForKey(raw_key), 
                                in, out, iv);
}

bool SslAESUtil::CBCDecrypt(const std::string& raw_key,
                            const std::string& iv,
                            io::InputStream* in,
                            io::OutputStream* out) {
  return CryptInternalNoCounter(kDecrypt, raw_key, CBCGetCipherForKey(raw_key),
                                in, out, iv);
}

bool SslAESUtil::ECBEncrypt(const std::string& raw_key,
                            io::InputStream* in,
                            io::OutputStream* out) {
  return CryptInternalNoCounter(kEncrypt, raw_key, ECBGetCipherForKey(raw_key),
                                in, out);
}

bool SslAESUtil::ECBDecrypt(const std::string& raw_key,
                            io::InputStream* in,
                            io::OutputStream* out) {
  return CryptInternalNoCounter(kDecrypt, raw_key, ECBGetCipherForKey(raw_key),
                                in, out);
}

} // namespace crypto
