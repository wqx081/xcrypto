#ifndef CRYPTO_IO_IO_UTIL_H_
#define CRYPTO_IO_IO_UTIL_H_

#include "io/input_stream.h"
#include "io/output_stream.h"

namespace io {

class IOUtil {
 public:
  static int ReadFromInput(InputStream* input,
                           void* data,
                           int size);
  static bool WriteToOutput(OutputStream* output,
                            const void* data,
                            int size);
  static bool PeekInput(InputStream* input);

 private:
  IOUtil();
};

} // namespace io
#endif // CRYPTO_IO_IO_UTIL_H_
