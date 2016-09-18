#include "io/file_output_stream.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <algorithm>

#include <glog/logging.h>

namespace io {

namespace {

int close_no_eintr(int fd) {
  int result;
  do {
    result = close(fd);
  } while (result < 0 && errno == EINTR);
  return result;
}

}  // namespace


FileOutputStream::FileOutputStream(int file_descriptor, int block_size)
  : copying_output_(file_descriptor),
    impl_(&copying_output_, block_size) {
}

FileOutputStream::~FileOutputStream() {
  impl_.Flush();
}

bool FileOutputStream::Close() {
  bool flush_succeeded = impl_.Flush();
  return copying_output_.Close() && flush_succeeded;
}

bool FileOutputStream::Flush() {
  return impl_.Flush();
}

bool FileOutputStream::Next(void** data, int* size) {
  return impl_.Next(data, size);
}

void FileOutputStream::BackUp(int count) {
  impl_.BackUp(count);
}

int64_t FileOutputStream::ByteCount() const {
  return impl_.ByteCount();
}

FileOutputStream::CopyFileOutputStream::CopyFileOutputStream(
    int file_descriptor)
  : file_(file_descriptor),
    close_on_delete_(false),
    is_closed_(false),
    errno_(0) {
}

FileOutputStream::CopyFileOutputStream::~CopyFileOutputStream() {
  if (close_on_delete_) {
    if (!Close()) {
      LOG(ERROR) << "close() failed: " << strerror(errno_);
    }
  }
}

bool FileOutputStream::CopyFileOutputStream::Close() {
  CHECK(!is_closed_);

  is_closed_ = true;
  if (close_no_eintr(file_) != 0) {
    errno_ = errno;
    return false;
  } 
  
  return true;
} 

bool FileOutputStream::CopyFileOutputStream::Write(
    const void* buffer, int size) {
  CHECK(!is_closed_);
  int total_written = 0;
  
  const uint8_t* buffer_base = reinterpret_cast<const uint8_t*>(buffer);
  
  while (total_written < size) {
    int bytes;
    do {
      bytes = write(file_, buffer_base + total_written, size - total_written);
    } while (bytes < 0 && errno == EINTR);
    
    if (bytes <= 0) {
      
      if (bytes < 0) {
        errno_ = errno;
      } 
      return false;
    } 
    total_written += bytes;
  } 
  
  return true;
} 

} // namespace io
