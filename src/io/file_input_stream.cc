#include "io/file_input_stream.h"
#include "base/stl_util.h"

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


FileInputStream::FileInputStream(int file_descriptor, int block_size)
  : copying_input_(file_descriptor),
    impl_(&copying_input_, block_size) {
}

FileInputStream::~FileInputStream() {}

bool FileInputStream::Close() {
  return copying_input_.Close();
}

bool FileInputStream::Next(const void** data, int* size) {
  return impl_.Next(data, size);
}

void FileInputStream::BackUp(int count) {
  impl_.BackUp(count);
}

bool FileInputStream::Skip(int count) {
  return impl_.Skip(count);
}

int64_t FileInputStream::ByteCount() const {
  return impl_.ByteCount();
}

FileInputStream::CopyFileInputStream::CopyFileInputStream(
    int file_descriptor)
  : file_(file_descriptor),
    close_on_delete_(false),
    is_closed_(false),
    errno_(0),
    previous_seek_failed_(false) {
}

FileInputStream::CopyFileInputStream::~CopyFileInputStream() {
  if (close_on_delete_) {
    if (!Close()) {
      LOG(ERROR) << "close() failed: " << strerror(errno_);
    }
  }
}

bool FileInputStream::CopyFileInputStream::Close() {
  CHECK(!is_closed_);

  is_closed_ = true;
  if (close_no_eintr(file_) != 0) {
    errno_ = errno;
    return false;
  } 
  
  return true;
} 

int FileInputStream::CopyFileInputStream::Read(void* buffer, int size) {
  CHECK(!is_closed_);
  
  int result;
  do {
    result = read(file_, buffer, size);
  } while (result < 0 && errno == EINTR);
  
  if (result < 0) {
    errno_ = errno;
  } 
  
  return result;
} 

int FileInputStream::CopyFileInputStream::Skip(int count) {
  CHECK(!is_closed_);
  
  if (!previous_seek_failed_ &&
      lseek(file_, count, SEEK_CUR) != (off_t)-1) {
    return count;
  } else { 
    previous_seek_failed_ = true;
    return CopyInputStream::Skip(count);
  } 
} 

} // namespace io
