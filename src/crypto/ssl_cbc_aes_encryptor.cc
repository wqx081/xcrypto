#include "crypto/ssl_cbc_aes_encryptor.h"
#include "crypto/ssl_aes_util.h"

#include <memory>
#include <glog/logging.h>

namespace crypto {

SslCbcAESEncryptor::SslCbcAESEncryptor(const std::string& raw_key,
                                       const std::string& iv)
    : iv_(iv),
      key_(AESKey::FromBytesBuffer(raw_key.data(), raw_key.size())) {
}

SslCbcAESEncryptor::~SslCbcAESEncryptor() {}

base::Status SslCbcAESEncryptor::Encrypt(io::InputStream* in,
                                         io::OutputStream* out) {
  if (SslAESUtil::CBCEncrypt(key_->raw_key(), iv_, in, out)) {
    return base::Status::OK;
  }
  return base::Status(base::error::INTERNAL, "Maybe error occur in SSL");
}

base::Status SslCbcAESEncryptor::Decrypt(io::InputStream* in,
                                         io::OutputStream* out) {
  if (SslAESUtil::CBCDecrypt(key_->raw_key(), iv_, in, out)) {
    return base::Status::OK;
  }
  return base::Status(base::error::INTERNAL, "Maybe error occur in SSL");
}

} // namespace crypto
