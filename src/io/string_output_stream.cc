#include "io/string_output_stream.h"
#include "base/stl_util.h"

#include <glog/logging.h>

namespace io {

StringOutputStream::StringOutputStream(string* target)
  : target_(target) {
}
  
StringOutputStream::~StringOutputStream() {
}
  
bool StringOutputStream::Next(void** data, int* size) {
  CHECK(target_ != NULL);
  size_t old_size = target_->size();
  
  if (old_size < target_->capacity()) {
    base::STLStringResizeUninitialized(target_, target_->capacity());
  } else {
    if (old_size > std::numeric_limits<int>::max() / 2) {
      LOG(ERROR) << "Cannot allocate buffer larger than kint32max for "
                   << "StringOutputStream.";
      return false; 
    } 
    base::STLStringResizeUninitialized(
          target_,
          std::max(old_size * 2,
                   (size_t)kMinimumSize + 0));  // "+ 0" works around GCC4 weirdness.
  }              
    
  *data = mutable_string_data(target_) + old_size;
  *size = target_->size() - old_size;
  return true;
} 

void StringOutputStream::BackUp(int count) {
  CHECK_GE(count, 0);
  CHECK(target_ != NULL);
  CHECK_LE(count, target_->size());
  target_->resize(target_->size() - count);
}
  
int64_t StringOutputStream::ByteCount() const {
  CHECK(target_ != NULL);
  return target_->size();
}
  
void StringOutputStream::SetString(string* target) {
  target_ = target;
}

LazyStringOutputStream::LazyStringOutputStream(
    base::ResultCallback<string*>* callback)
  : StringOutputStream(NULL),
    callback_(CHECK_NOTNULL(callback)),
    string_is_set_(false) {
}
  
LazyStringOutputStream::~LazyStringOutputStream() {
}
  
bool LazyStringOutputStream::Next(void** data, int* size) {
  if (!string_is_set_) {
    SetString(callback_->Run());
    string_is_set_ = true;
  }
  return StringOutputStream::Next(data, size);
}
  
int64_t LazyStringOutputStream::ByteCount() const {
  return string_is_set_ ? StringOutputStream::ByteCount() : 0;
}

} // namespace io
