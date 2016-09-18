#ifndef CRYPTO_IO_COPY_OUTPUT_STREAM_H_
#define CRYPTO_IO_COPY_OUTPUT_STREAM_H_

#include "base/macros.h"
#include "io/output_stream.h"

#include <memory>

namespace io {

class CopyOutputStream {
 public:
  virtual ~CopyOutputStream();
  
  virtual bool Write(const void* buffer, int size) = 0;
};

class CopyOutputStreamAdaptor : public OutputStream {
 public:
  explicit CopyOutputStreamAdaptor(CopyOutputStream* copying_stream,
                                   int block_size = -1);
  ~CopyOutputStreamAdaptor();
  
  bool Flush();
  
  void SetOwnsCopyingStream(bool value) { owns_copying_stream_ = value; }
  
  // From OutputStream
  bool Next(void** data, int* size);
  void BackUp(int count);
  int64_t ByteCount() const;
  
 private:
  bool WriteBuffer();
  void AllocateBufferIfNeeded();
  void FreeBuffer();
  
  CopyOutputStream* copying_stream_;
  bool owns_copying_stream_;
  
  bool failed_;
  
  int64_t position_;
  
  std::unique_ptr<uint8_t[]> buffer_;
  const int buffer_size_;
  
  int buffer_used_;
  
  DISALLOW_COPY_AND_ASSIGN(CopyOutputStreamAdaptor);
};

} // namespace io
#endif // CRYPTO_IO_COPY_OUTPUT_STREAM_H_
