#include "unittestes/io/io_test.h"
#include "io/array_input_stream.h"
#include "io/array_output_stream.h"

namespace io {

TEST_F(IoTest, ArrayIo) {
  const int kBufferSize = 256;
  uint8_t buffer[kBufferSize];
  
  for (int i = 0; i < kBlockSizeCount; i++) {
    for (int j = 0; j < kBlockSizeCount; j++) {
      int size;
      {
          ArrayOutputStream output(buffer, kBufferSize, kBlockSizes[i]);
          size = WriteStuff(&output);
      }
      {
          ArrayInputStream input(buffer, size, kBlockSizes[j]);
          ReadStuff(&input);
      }
    }
  }
}


} // namespace io
