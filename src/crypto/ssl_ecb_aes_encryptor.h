#ifndef CYPRTO_SSL_ECB_AES_ENCRYPTOR_H_
#define CYPRTO_SSL_ECB_AES_ENCRYPTOR_H_

#include "base/macros.h"
#include "crypto/aes_key.h"
#include "crypto/aes_encryptor.h"

#include <string>

namespace crypto {

class SslEcbAESEncryptor : public AESEncryptor {
 public:
  SslEcbAESEncryptor(const std::string& raw_key);
  virtual ~SslEcbAESEncryptor() override;

  // From AESEncryptor   
  virtual base::Status Encrypt(io::InputStream* input,
                               io::OutputStream* output) override; 
  virtual base::Status Decrypt(io::InputStream* input,
                               io::OutputStream* output) override;

 private:
  std::unique_ptr<AESKey> key_;
};

} // namespace crypto
#endif // CYPRTO_SSL_ECB_AES_ENCRYPTOR_H_
