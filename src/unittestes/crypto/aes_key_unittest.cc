#include "crypto/aes_key.h"

#include "strings/string_encode.h"

#include <glog/logging.h>

#include <gtest/gtest.h>

namespace crypto {

TEST(AESKey, Constructor) {
  std::unique_ptr<AESKey> key1 = AESKey::Create(128);
  EXPECT_TRUE(key1);
  std::string hex_string1 = key1->ToHexString();
  LOG(INFO) << "hex_string1: " << hex_string1;
  std::unique_ptr<AESKey> key2 = AESKey::FromHexString(hex_string1);
  std::string hex_string2 = key2->ToHexString();
  LOG(INFO) << "hex_string2: " << hex_string2;
  EXPECT_EQ(hex_string1, hex_string2);
}

}
