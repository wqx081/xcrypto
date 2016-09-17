#include "io/array_output_stream.h"
#include <glog/logging.h>

namespace io {

ArrayOutputStream::ArrayOutputStream(void* data, int size, int block_size)
    : data_(reinterpret_cast<uint8_t*>(data)),
      size_(size),
      block_size_(block_size > 0 ? block_size : size),
      position_(0),
      last_returned_size_(0) {
}

ArrayOutputStream::~ArrayOutputStream() {}

bool ArrayOutputStream::Next(void** data, int* size) {
  if (position_ < size_) {
    last_returned_size_ = std::min(block_size_, size_ - position_);
    *data = data_ + position_;
    *size = last_returned_size_;
    position_ += last_returned_size_;
    return true;
  } else {
    last_returned_size_ = 0;
    return false;
  }
}

void ArrayOutputStream::BackUp(int count) {
  CHECK_GT(last_returned_size_, 0) 
	  << "BackUp() can only be called after a successful Next().";
  CHECK_LE(count, last_returned_size_);
  CHECK_GE(count, 0);
  position_ -= count;
  last_returned_size_ = 0;
}

int64_t ArrayOutputStream::ByteCount() const {
  return position_;
}

} // namespace io
