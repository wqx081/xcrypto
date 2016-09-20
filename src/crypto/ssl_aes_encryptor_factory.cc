#include "crypto/aes_encryptor.h"
#include "crypto/ssl_cbc_aes_encryptor.h"
#include "crypto/ssl_ecb_aes_encryptor.h"

namespace crypto {

class SslAESFactory : public AESFactory {
 public:
  SslAESFactory() {}
  virtual ~SslAESFactory() override {}

  // From AESFactory
  virtual std::unique_ptr<AESEncryptor> CreateCBC(std::unique_ptr<AESKey>& key,
                                                  const std::string& iv) override {
    std::unique_ptr<SslCbcAESEncryptor> ret(new SslCbcAESEncryptor(key->raw_key(), iv));
    return std::move(ret);
  }
  
  virtual std::unique_ptr<AESEncryptor> CreateECB(std::unique_ptr<AESKey>& key) override {
    std::unique_ptr<SslEcbAESEncryptor> ret(new SslEcbAESEncryptor(key->raw_key()));
    return std::move(ret);
  }

  virtual std::unique_ptr<AESEncryptor> CreateCTR(std::unique_ptr<AESKey>& key) override {
    (void)key;
    //TODO(wqx):
    return nullptr;
  }

  virtual bool AcceptsOptions(const std::string& encryptor_type) override {
    return encryptor_type == "ssl_aes";
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SslAESFactory);
};

namespace { // register

class SslAESRegistrar {
 public:
  SslAESRegistrar() {
    AESFactory::Register("ssl_aes", new SslAESFactory());
  }
};
static SslAESRegistrar registrar;
} // namespace
} // namespace crypto
