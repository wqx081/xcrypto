#ifndef CRYPTO_AES_KEY_H_
#define CRYPTO_AES_KEY_H_

#include "base/macros.h"

#include <memory>
#include <string>

namespace crypto {

class AESKey {
 public:
  virtual ~AESKey();

  static std::unique_ptr<AESKey> Create(int key_size_in_bits);
  static std::unique_ptr<AESKey> FromHexString(const std::string& hex_string); 
  static std::unique_ptr<AESKey> FromBytesBuffer(const char* buffer, int len);

  std::string ToHexString() const;
  std::string raw_key() const;

 private:
  AESKey();
  std::string key_;

  DISALLOW_COPY_AND_ASSIGN(AESKey);
};

} // namespace base
#endif // CRYPTO_AES_KEY_H_
