#ifndef XCRYPTO_IO_ARRAY_OUTPUT_STREAM_H_
#define XCRYPTO_IO_ARRAY_OUTPUT_STREAM_H_

#include "io/output_stream.h"

namespace io {

class ArrayOutputStream : public OutputStream {
 public:
  ArrayOutputStream(void* data, int size, int block_size = -1);
  ~ArrayOutputStream();

  // From OutputStream
  virtual bool Next(void** data, int* size) override;
  virtual void BackUp(int count) override;
  virtual int64_t ByteCount() const override;

 private:
  uint8_t* const data_;
  const int size_;
  const int block_size_;

  int position_;
  int last_returned_size_;

  DISALLOW_COPY_AND_ASSIGN(ArrayOutputStream);
};

} // namespace io
#endif // XCRYPTO_IO_ARRAY_OUTPUT_STREAM_H_
