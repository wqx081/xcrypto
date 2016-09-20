#ifndef CRYPTO_AES_ENCRYPTOR_H_
#define CRYPTO_AES_ENCRYPTOR_H_

#include "base/macros.h"
#include "base/status.h"
#include "crypto/aes_key.h"

#include <string>
#include <memory>

namespace io {
class InputStream;
class OutputStream;
} // namespace io

namespace crypto {

class AESEncryptor {
 public:
  virtual ~AESEncryptor() {}  

  virtual base::Status Encrypt(io::InputStream* input,
                               io::OutputStream* output) = 0;

  virtual base::Status Decrypt(io::InputStream* input,
                               io::OutputStream* output) = 0;
};

class AESFactory {
 public:
  virtual ~AESFactory() {}

  virtual std::unique_ptr<AESEncryptor> CreateCBC(std::unique_ptr<AESKey>& key, 
                                                  const std::string& iv) = 0;
  virtual std::unique_ptr<AESEncryptor> CreateECB(std::unique_ptr<AESKey>& key) = 0;
  virtual std::unique_ptr<AESEncryptor> CreateCTR(std::unique_ptr<AESKey>& key) = 0;
  virtual bool AcceptsOptions(const std::string& encryptor_type) = 0;

  static void Register(const std::string& encryptor_type, AESFactory* factory);
  static base::Status GetFactory(const std::string& encryptor_type,
                                 AESFactory** out_factory);
};

} // namespace crypto
#endif // CRYPTO_AES_ENCRYPTOR_H_
