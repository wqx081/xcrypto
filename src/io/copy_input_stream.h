#ifndef CRYPTO_IO_COPY_INPUT_STREAM_H_
#define CRYPTO_IO_COPY_INPUT_STREAM_H_

#include "io/input_stream.h"

#include <memory>

namespace io {

class CopyInputStream {
 public:
  virtual ~CopyInputStream();

  virtual int Read(void* buffer, int size) = 0;
  virtual int Skip(int count);
};

class CopyInputStreamAdaptor : public InputStream {
 public:
  explicit CopyInputStreamAdaptor(CopyInputStream* copy_stream,
                                  int block_size = -1); 
  ~CopyInputStreamAdaptor();

  void SetOwnsCopyStream(bool value) { owns_copy_stream_ = value; }

  // From InputStream
  bool Next(const void** data, int* size);
  void BackUp(int count);
  bool Skip(int count);
  int64_t ByteCount() const;

 private:
  void AllocateBufferIfNeeded();
  void FreeBuffer();

  CopyInputStream* copy_stream_;
  bool owns_copy_stream_;

  bool failed_;

  int64_t position_;

  std::unique_ptr<uint8_t[]> buffer_;
  const int buffer_size_;

  int buffer_used_;
  int backup_bytes_;

  DISALLOW_COPY_AND_ASSIGN(CopyInputStreamAdaptor);
};


} // namespace io

#endif // CRYPTO_IO_COPY_INPUT_STREAM_H_
