#include "crypto/aes_encryptor.h"

#include "io/string_input_stream.h"
#include "io/string_output_stream.h"

#include "strings/string_encode.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

namespace crypto {

TEST(SslAESFactory, CBCTest) {
  AESFactory* factory;
  //Not supported SM4
  EXPECT_FALSE(AESFactory::GetFactory("sm4_aes", &factory).ok());

  // Support SSL AES
  EXPECT_TRUE(AESFactory::GetFactory("ssl_aes", &factory).ok());

  std::unique_ptr<AESKey> key = AESKey::Create(128);
  EXPECT_TRUE(key);
  const std::string iv("16 bytes init iv");
  const std::string text("Hello, World"); 
  std::string cipher;
  std::unique_ptr<AESEncryptor> cbc_encryptor = factory->CreateCBC(key, iv);
  EXPECT_TRUE(cbc_encryptor);  

  // CBC Encrypt
  io::StringInputStream input(text.data(), text.size());
  io::StringOutputStream output(&cipher);
  EXPECT_TRUE(cbc_encryptor->Encrypt(&input, &output).ok());
  LOG(INFO) << "cbc cipher: " << strings::HexEncode(cipher);

  // CBC Decrypt
  std::string plain;
  io::StringInputStream input1(cipher.data(), cipher.size());
  io::StringOutputStream output1(&plain);
  EXPECT_TRUE(cbc_encryptor->Decrypt(&input1, &output1).ok());
  LOG(INFO) << "cbc plain: " << plain;

}

TEST(SslAESFactory, ECBTest) {
  AESFactory* factory;
  EXPECT_TRUE(AESFactory::GetFactory("ssl_aes", &factory).ok());

  std::unique_ptr<AESKey> key = AESKey::Create(128);
  EXPECT_TRUE(key);
  const std::string text("1234567890123456"); 
  std::string cipher;
  std::unique_ptr<AESEncryptor> ecb_encryptor = factory->CreateECB(key);
  EXPECT_TRUE(ecb_encryptor);  
  
  // ECB Encrypt
  io::StringInputStream input(text.data(), text.size());
  io::StringOutputStream output(&cipher);
  EXPECT_TRUE(ecb_encryptor->Encrypt(&input, &output).ok());
  LOG(INFO) << "ecb cipher: " << strings::HexEncode(cipher);

  // ECB Decrypt
  std::string plain;
  io::StringInputStream input1(cipher.data(), cipher.size());
  io::StringOutputStream output1(&plain);
  EXPECT_TRUE(ecb_encryptor->Decrypt(&input1, &output1).ok());
  LOG(INFO) << "ecb plain: " << plain;

}

} // namespace crypto
