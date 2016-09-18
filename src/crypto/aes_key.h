#ifndef CRYPTO_AES_KEY_H_
#define CRYPTO_AES_KEY_H_

#include "base/macros.h"

#include <memory>
#include <string>

namespace crypto {

class AESKey {
 public:
  virtual ~AESKey() {}

  virtual std::unique_ptr<AESKey> Create(int key_size_in_bits) = 0;
  virtual std::unique_ptr<AESKey> FromHexString(const std::string& hex_string) = 0;
  virtual std::unique_ptr<AESKey> FromBytesBuffer(const char* buffer, int len) = 0;
  virtual std::string ToHexString() const = 0;
  virtual std::string raw_key() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AESKey);
};

} // namespace base
#endif // CRYPTO_AES_KEY_H_
