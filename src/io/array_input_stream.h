#ifndef XCRYPTO_IO_ARRAY_INPUT_STREAM_H_
#define XCRYPTO_IO_ARRAY_INPUT_STREAM_H_

#include "base/macros.h"
#include "io/input_stream.h"

namespace io {

class ArrayInputStream : public InputStream {
 public:
  ArrayInputStream(const void* data, int size, int block_size = -1);
  virtual ~ArrayInputStream() override;

  virtual bool Next(const void** data, int* size) override;
  virtual void BackUp(int count) override;
  virtual bool Skip(int count) override;
  virtual int64_t ByteCount() const override;

 private:
  const uint8_t* const data_;
  const int size_;
  const int block_size_;

  int position_;
  int last_returned_size_;

  DISALLOW_COPY_AND_ASSIGN(ArrayInputStream);
};

} // namespace io
#endif // XCRYPTO_IO_ARRAY_INPUT_STREAM_H_
