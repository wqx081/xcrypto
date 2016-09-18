#include "crypto/aes_encryptor.h"

#include <unordered_map>
#include <mutex>

#include <glog/logging.h>

using base::Status;

namespace crypto {

namespace {

std::mutex* get_aes_factory_lock() {
  static std::mutex server_factory_lock;
  return &server_factory_lock;
}

typedef std::unordered_map<std::string, AESFactory*> AESFactories;
AESFactories* aes_factories() {
  static AESFactories* factories = new AESFactories;
  return factories;
}

} // namespace

// static
void AESFactory::Register(const std::string& encryptor_type,
                          AESFactory* factory) {
  std::unique_lock<std::mutex> l(*get_aes_factory_lock());
  if (!aes_factories()->insert({encryptor_type, factory}).second) {
    LOG(ERROR) << "Two aes factories are begin registered under "
               << encryptor_type;
  }
}

// static
Status AESFactory::GetFactory(const std::string& encryptor_type,
                              AESFactory** out_factory) {
  std::unique_lock<std::mutex> l(*get_aes_factory_lock());
  for (const auto& aes_factory : *aes_factories()) {
    if (aes_factory.second->AcceptsOptions(encryptor_type)) {
      *out_factory = aes_factory.second;
      return Status::OK;
    }
  } 
  return Status(base::error::NOT_FOUND,
                "No AES Encryptor factory registered for the given encryptor_type: "
                + encryptor_type);
}

} // namespace crypto
