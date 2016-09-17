//@Author: WangQixiang (wangqx at mpreader.com)
//
// Example:
//
//   int fd = open("/path/to/inputfile", O_RDONLY);
//   InputStream* input = new FileInputStream(fd);
//
//   const void* buffer;
//   int size;
//   while (input->Next(&buffer, &size)) {
//     cout.write(buffer, size);
//   }
//
//   delete input;
//   close(fd);
//
#ifndef XCRYPTO_IO_INPUT_STREAM_H_
#define XCRYPTO_IO_INPUT_STREAM_H_
#include <string>
#include "base/macros.h"

namespace io {

class InputStream {
 public:
  InputStream() {}
  virtual ~InputStream();

  virtual bool Next(const void** data, int* size) = 0;
  virtual void BackUp(int count) = 0;
  virtual bool Skip(int count) = 0;
  virtual int64_t ByteCount() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(InputStream);
};

} // namespace io
#endif // XCRYPTO_IO_INPUT_STREAM_H_
