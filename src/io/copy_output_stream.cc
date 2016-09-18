#include "io/copy_output_stream.h"

#include <glog/logging.h>

namespace io {

static const int kDefaultBlockSize = 8192;

CopyOutputStream::~CopyOutputStream() {}
  
CopyOutputStreamAdaptor::CopyOutputStreamAdaptor(
      CopyOutputStream* copying_stream, int block_size)
    : copying_stream_(copying_stream),
      owns_copying_stream_(false),
      failed_(false),
      position_(0),
      buffer_size_(block_size > 0 ? block_size : kDefaultBlockSize),
      buffer_used_(0) {
}
  
CopyOutputStreamAdaptor::~CopyOutputStreamAdaptor() {
  WriteBuffer();
    if (owns_copying_stream_) {
      delete copying_stream_;
    }
}
  
bool CopyOutputStreamAdaptor::Flush() {
    return WriteBuffer();
}
  
bool CopyOutputStreamAdaptor::Next(void** data, int* size) {
    if (buffer_used_ == buffer_size_) {
      if (!WriteBuffer()) return false;
    }
  
    AllocateBufferIfNeeded();
  
    *data = buffer_.get() + buffer_used_;
    *size = buffer_size_ - buffer_used_;
    buffer_used_ = buffer_size_;
    return true;
}
  
void CopyOutputStreamAdaptor::BackUp(int count) {
    CHECK_GE(count, 0);
    CHECK_EQ(buffer_used_, buffer_size_)
      << " BackUp() can only be called after Next().";
    CHECK_LE(count, buffer_used_)
      << " Can't back up over more bytes than were returned by the last call"
         " to Next().";
  
    buffer_used_ -= count;
  }
  
  int64_t CopyOutputStreamAdaptor::ByteCount() const {
    return position_ + buffer_used_;
  }

  bool CopyOutputStreamAdaptor::WriteBuffer() {
    if (failed_) {
      return false;
    }
  
    if (buffer_used_ == 0) return true;
  
    if (copying_stream_->Write(buffer_.get(), buffer_used_)) {
      position_ += buffer_used_;
      buffer_used_ = 0;
      return true;
    } else {
      failed_ = true;
      FreeBuffer();
      return false;
    }
  }
  
void CopyOutputStreamAdaptor::AllocateBufferIfNeeded() {
    if (buffer_ == NULL) {
      buffer_.reset(new uint8_t[buffer_size_]);
    }
  }
  
  void CopyOutputStreamAdaptor::FreeBuffer() {
    buffer_used_ = 0;
    buffer_.reset();
  }

} // namespace io
