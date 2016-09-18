#include "io/copy_input_stream.h"

#include <glog/logging.h>

namespace io {

static const int kDefaultBlockSize = 8192;

CopyInputStream::~CopyInputStream() {}
  
int CopyInputStream::Skip(int count) {
  char junk[4096];
  int skipped = 0;
  while (skipped < count) {
    int bytes =
    Read(junk, std::min(count - skipped, static_cast<int>(sizeof(junk))));
    if (bytes <= 0) {
      return skipped;
    }
    skipped += bytes;
  }
  return skipped;
}
  
CopyInputStreamAdaptor::CopyInputStreamAdaptor(
      CopyInputStream* copying_stream, int block_size)
    : copy_stream_(copying_stream),
      owns_copy_stream_(false),
      failed_(false),
      position_(0),
      buffer_size_(block_size > 0 ? block_size : kDefaultBlockSize),
      buffer_used_(0),
      backup_bytes_(0) {
}
  
CopyInputStreamAdaptor::~CopyInputStreamAdaptor() {
  if (owns_copy_stream_) {
    delete copy_stream_;
  }
}
  
bool CopyInputStreamAdaptor::Next(const void** data, int* size) {
    if (failed_) {
      return false;
    }
  
    AllocateBufferIfNeeded();
  
    if (backup_bytes_ > 0) {
      *data = buffer_.get() + buffer_used_ - backup_bytes_;
      *size = backup_bytes_;
      backup_bytes_ = 0;
      return true;
    }
  
    buffer_used_ = copy_stream_->Read(buffer_.get(), buffer_size_);
    if (buffer_used_ <= 0) {
      if (buffer_used_ < 0) {
        failed_ = true;
      }
      FreeBuffer();
      return false;
    }
    position_ += buffer_used_;
  
    *size = buffer_used_;
    *data = buffer_.get();
    return true;
}
  
void CopyInputStreamAdaptor::BackUp(int count) {
    CHECK(backup_bytes_ == 0 && buffer_.get() != NULL)
      << " BackUp() can only be called after Next().";
    CHECK_LE(count, buffer_used_)
      << " Can't back up over more bytes than were returned by the last call"
         " to Next().";
    CHECK_GE(count, 0)
      << " Parameter to BackUp() can't be negative.";
  
    backup_bytes_ = count;
}

bool CopyInputStreamAdaptor::Skip(int count) {
    CHECK_GE(count, 0);
  
    if (failed_) {
      return false;
    }
  
    if (backup_bytes_ >= count) {
      backup_bytes_ -= count;
      return true;
    }
  
    count -= backup_bytes_;
    backup_bytes_ = 0;
  
    int skipped = copy_stream_->Skip(count);
    position_ += skipped;
    return skipped == count;
}
  
int64_t CopyInputStreamAdaptor::ByteCount() const {
  return position_ - backup_bytes_;
}
  
void CopyInputStreamAdaptor::AllocateBufferIfNeeded() {
  if (buffer_.get() == NULL) {
    buffer_.reset(new uint8_t[buffer_size_]);
  }
}
  
void CopyInputStreamAdaptor::FreeBuffer() {
  CHECK_EQ(backup_bytes_, 0);
  buffer_used_ = 0;
  buffer_.reset();
}

} // namespace io
