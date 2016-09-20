#ifndef CRYPTO_SSL_AES_UTIL_H_
#define CRYPTO_SSL_AES_UTIL_H_

#include "base/macros.h"

#include <string>

namespace io {
class InputStream;
class OutputStream;
} // namespace io

namespace crypto {

class SslAESUtil {
 public:
  static bool CBCEncrypt(const std::string& raw_key,
                         const std::string& iv,
                         io::InputStream* in,
                         io::OutputStream* out);
  static bool CBCDecrypt(const std::string& raw_key,
                         const std::string& iv,
                         io::InputStream* in,
                         io::OutputStream* out);

  static bool ECBEncrypt(const std::string& raw_key,
                         io::InputStream* in,
                         io::OutputStream* out);
  static bool ECBDecrypt(const std::string& raw_key,
                         io::InputStream* in,
                         io::OutputStream* out);

  // TODO(wqx):
  // CTR
 private:
  SslAESUtil() = delete;
  DISALLOW_COPY_AND_ASSIGN(SslAESUtil);
};

} // namespace crypto
#endif // CRYPTO_SSL_AES_UTIL_H_
