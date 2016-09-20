#ifndef CRYPTO_CBC_AES_ENCRYPTOR_H_
#define CRYPTO_CBC_AES_ENCRYPTOR_H_

#include "base/macros.h"
#include "crypto/aes_key.h"
#include "crypto/aes_encryptor.h"

#include <string>

namespace crypto {

class SslCbcAESEncryptor : public AESEncryptor {
 public:
  SslCbcAESEncryptor(const std::string& raw_key, const std::string& iv);
  virtual ~SslCbcAESEncryptor() override;

  // From AESEncryptor;
  virtual base::Status Encrypt(io::InputStream* input,
                               io::OutputStream* output) override;
  virtual base::Status Decrypt(io::InputStream* input,
                               io::OutputStream* output) override;

 private:
  std::string iv_;
  std::unique_ptr<AESKey> key_;
};

} // namespace crypto
#endif // CRYPTO_CBC_AES_ENCRYPTOR_H_
