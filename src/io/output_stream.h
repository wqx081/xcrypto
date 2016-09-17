//@author: WangQixiang (wangqx at mpreader.com)
//
// Example:
//
//   int infd = open("/path/to/input/file", O_RDONLY);
//   int outfd = open("/path/to/output/file", O_WRONLY);
//   OutputStream* output = new FileOutputStream(outfd);
//
//   void* buffer;
//   int size;
//   while (output->Next(&buffer, &size)) {
//     int bytes = read(infd, buffer, size);
//     if (bytes < size) {
//       // EOF
//       output->BackUp(size - bytes);
//       break;
//     }
//   }
//
//   delete output;
//   close(infd);
//   close(outfd);
//
#ifndef XCRYPTO_IO_OUTPUT_STREAM_H_
#define XCRYPTO_IO_OUTPUT_STREAM_H_
#include "base/macros.h"

#include <string>

namespace io {

class OutputStream {
 public:
  OutputStream() {}
  virtual ~OutputStream();

  virtual bool Next(void** data, int* size) = 0;
  virtual void BackUp(int count) = 0;
  virtual int64_t ByteCount() const = 0;

  virtual bool WriteAliasedRaw(const void* data, int size);
  virtual bool AllowsAliasing() const { return false; }

 private:
  DISALLOW_COPY_AND_ASSIGN(OutputStream);
};

} // namespace io
#endif // XCRYPTO_IO_OUTPUT_STREAM_H_
