#include "io/array_input_stream.h"

#include <glog/logging.h>

namespace io {

ArrayInputStream::ArrayInputStream(const void* data,
                                   int size,
				   int block_size)
    : data_(reinterpret_cast<const uint8_t*>(data)),
      size_(size),
      block_size_(block_size > 0 ? block_size : size),
      position_(0),
      last_returned_size_(0) {
}

ArrayInputStream::~ArrayInputStream() {}

bool ArrayInputStream::Next(const void** data, int* size) {
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

void ArrayInputStream::BackUp(int count) {
  CHECK_GT(last_returned_size_, 0)
	  << "BackUp() can only be called after a successful Next()";
  CHECK_LE(count, last_returned_size_);
  CHECK_GE(count, 0);
  position_ -= count;
  last_returned_size_ = 0;

}

bool ArrayInputStream::Skip(int count) {
  CHECK_GE(count, 0);
  last_returned_size_ = 0;
  if (count > size_ - position_) {
    position_ = size_;
    return false;
  } else {
    position_ += count;
    return true;
  }
}

int64_t ArrayInputStream::ByteCount() const {
  return position_;
}

} // namespace io
