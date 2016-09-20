#include "crypto/aes_encryptor.h"

#if 0
#include "io/input_stream.h"
#include "io/output_stream.h"

#include <glog/logging.h>

using io::InputStream;
using io::OutputStream;
using base::Status;

namespace crypto {

// CBC
class SslCBCEncryptor : public AESEncryptor {
 public:
  SslCBCEncryptor() {}
  virtual ~SslCBCEncryptor() {}

  // From AESEncryptor  
  virtual Status Encrypt(const std::unique_ptr<AESKey>& key,
                         const io::InputStream* input_stream,
                         io::OutputStream* output_stream) override;
  virtual Status Decrypt(const std::unique_ptr<AESKey>& key,
                         const io::InputStream* input_stream,
                         io::OutputStream* output_stream) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(SslCBCEncryptor);
};

// ECB
class SslECBEncryptor : public AESEncryptor {
 public:
  SslECBEncryptor() {}
  virtual ~SslECBEncryptor() {}

  // From AESEncryptor
  virtual Status Encrypt(const std::unique_ptr<AESKey>& key,
                         const io::InputStream* input_stream,
                         io::OutputStream* output_stream) override;
  virtual Status Decrypt(const std::unique_ptr<AESKey>& key,
                         const io::InputStream* input_stream,
                         io::OutputStream* output_stream) override;
 private:
  DISALLOW_COPY_AND_ASSIGN(SslECBEncryptor);
};

// CTR


// SslAESFactory

} // namespace crypto
#endif
