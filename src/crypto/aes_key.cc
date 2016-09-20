#include "crypto/aes_key.h"

#include "third_party/boringssl/include/openssl/evp.h"
#include "third_party/boringssl/include/openssl/rand.h"

#include "strings/string_util.h"
#include "strings/string_encode.h"
#include "crypto/openssl_util.h"

#include <stddef.h>
#include <stdint.h>

namespace crypto {

namespace {

bool IsValidBitsLen(int key_size_in_bits) {
  return key_size_in_bits == 128 ||
         key_size_in_bits == 256;
}

bool IsValidBytesLen(int key_size_in_bytes) {
  return IsValidBitsLen(key_size_in_bytes * 8);
}

} // namespace

AESKey::~AESKey() {}

// static
std::unique_ptr<AESKey> AESKey::Create(int key_size_in_bits) {
  if (!IsValidBitsLen(key_size_in_bits)) {
    return nullptr;
  }
  OpenSSLErrStackTracer err_tracer(FROM_HERE);
  std::unique_ptr<AESKey> key(new AESKey);

  size_t key_size_in_bytes = key_size_in_bits / 8;
  uint8_t* key_data = reinterpret_cast<uint8_t*>(
    strings::WriteInto(&key->key_, key_size_in_bytes + 1));

  int rv = RAND_bytes(key_data,
                      static_cast<int>(key_size_in_bytes));
  return rv == 1 ? std::move(key) : nullptr;
}

// static
std::unique_ptr<AESKey> AESKey::FromHexString(const std::string& hex_string) {
  std::string hex_bytes_string = strings::HexDecode(hex_string);
  if (hex_bytes_string.empty()) {
    return nullptr;
  }
  std::unique_ptr<AESKey> key(new AESKey);
  key->key_ = std::move(hex_bytes_string);
  return key;
}

// static
std::unique_ptr<AESKey> AESKey::FromBytesBuffer(const char* buffer,
                                                int len) {
  if (!IsValidBytesLen(len)) {
    return nullptr;
  }
  std::unique_ptr<AESKey> key(new AESKey);
  key->key_ = std::move(std::string(buffer, len));
  return key;
}

std::string AESKey::ToHexString() const {
  return strings::HexEncode(key_);
}

std::string AESKey::raw_key() const {
  return key_;
}

AESKey::AESKey() = default;

} // namespace crypto
