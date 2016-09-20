#include "crypto/ssl_ecb_aes_encryptor.h"
#include "crypto/ssl_aes_util.h"

#include <glog/logging.h>

namespace crypto {

SslEcbAESEncryptor::SslEcbAESEncryptor(const std::string& raw_key)
    : key_(AESKey::FromBytesBuffer(raw_key.data(), raw_key.length())) {
  DCHECK(!raw_key.empty());
}

SslEcbAESEncryptor::~SslEcbAESEncryptor() {}

base::Status SslEcbAESEncryptor::Encrypt(io::InputStream* input,
                                         io::OutputStream* output) {
  if (SslAESUtil::ECBEncrypt(key_->raw_key(), input, output)) {
    return base::Status::OK;
  }
  return base::Status(base::error::INTERNAL, "Maybe Error occur in SSL");
}

base::Status SslEcbAESEncryptor::Decrypt(io::InputStream* input,
                                         io::OutputStream* output) {
  if (SslAESUtil::ECBDecrypt(key_->raw_key(), input, output)) {
    return base::Status::OK;
  }
  return base::Status(base::error::INTERNAL, "Maybe Error occur in SSL");
}


} // namespace crypto
