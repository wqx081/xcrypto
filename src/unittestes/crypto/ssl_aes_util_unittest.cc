#include "crypto/ssl_aes_util.h"
#include "crypto/aes_key.h"

#include "crypto/openssl_util.h"

#include "io/string_input_stream.h"
#include "io/string_output_stream.h"

#include "strings/string_encode.h"

#include <memory>

#include <glog/logging.h>
#include <gtest/gtest.h>


namespace crypto {

namespace {

class crypto_register {
 public:
  crypto_register() {
    EnsureOpenSSLInit();
  }
};
static crypto_register crypto_register_;

} // namespace

TEST(CBCCrypt, Test) {

  std::unique_ptr<AESKey> key = AESKey::Create(128);
  EXPECT_TRUE(key);
  std::string raw_key = key->raw_key();
  EXPECT_FALSE(raw_key.empty());
  EXPECT_EQ(raw_key.size(), 16);

  const std::string iv("16 bytes init ve");
  const std::string text("1234567890123456");
  // Encrypt
  std::string cipher;
  io::StringInputStream input(text.data(), text.size());
  io::StringOutputStream output(&cipher);

  EXPECT_TRUE(SslAESUtil::CBCEncrypt(raw_key, iv, &input, &output));
  
  LOG(INFO) << "cipher: " << strings::HexEncode(cipher);
  // Decyrpt
  io::StringInputStream input1(cipher.data(), cipher.size());
  std::string plaint;
  io::StringOutputStream output1(&plaint);
  EXPECT_TRUE(SslAESUtil::CBCDecrypt(raw_key, iv, &input1, &output1));

  LOG(INFO) << "plaint: " << plaint;
  EXPECT_TRUE(true);
}

TEST(ECBCrypt, TEST) {
  std::unique_ptr<AESKey> key = AESKey::Create(256);
  EXPECT_TRUE(key);
  std::string raw_key = key->raw_key();
  EXPECT_FALSE(raw_key.empty());
  EXPECT_EQ(raw_key.size(), 32);

  // Encrypt
  const std::string text("Hello, World");
  std::string cipher;
  io::StringInputStream input(text.data(), text.size());
  io::StringOutputStream output(&cipher);

  EXPECT_TRUE(SslAESUtil::ECBEncrypt(raw_key, &input, &output));
  LOG(INFO) << "cipher: " << strings::HexEncode(cipher);
}



} // namespace crypto
