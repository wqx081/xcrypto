#include "crypto/ssl_ecb_aes_encryptor.h"
#include "crypto/aes_key.h"

#include "io/string_input_stream.h"
#include "io/string_output_stream.h"

#include "strings/string_encode.h"

#include <memory>

#include <glog/logging.h>
#include <gtest/gtest.h>

namespace crypto {

TEST(SslEcbAESEncryptor, Crypt) {
  std::unique_ptr<AESKey> key(AESKey::Create(128));
  EXPECT_TRUE(key);
  const std::string text("1234567890123456");
  LOG(INFO) << "raw_key: " << strings::HexEncode(key->raw_key());
  std::string cipher;
  std::unique_ptr<SslEcbAESEncryptor> ecb_encryptor(new SslEcbAESEncryptor(key->raw_key()));
  EXPECT_TRUE(ecb_encryptor);

  io::StringInputStream input(text.data(), text.size());
  io::StringOutputStream output(&cipher);

  // Encrypt
  EXPECT_TRUE(ecb_encryptor->Encrypt(&input, &output).ok());
  LOG(INFO) << "ECB Encrypt: " << strings::HexEncode(cipher);

  // Decrypt
  std::string plain;
  io::StringInputStream input1(cipher.data(), cipher.size());
  io::StringOutputStream output1(&plain);
  EXPECT_TRUE(ecb_encryptor->Decrypt(&input1, &output1).ok());
  LOG(INFO) << "ECB Decrypt: " << plain;

  EXPECT_TRUE(true);
}

} // namespace crypto
