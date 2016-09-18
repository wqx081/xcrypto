#ifndef CRYPTO_IO_FILE_INPUT_STREAM_H_
#define CRYPTO_IO_FILE_INPUT_STREAM_H_

#include "base/macros.h"
#include "io/input_stream.h"
#include "io/copy_input_stream.h"

namespace io {

class FileInputStream : public InputStream {
 public:
  explicit FileInputStream(int file_descriptor, int block_size = -1);
  ~FileInputStream();

  bool Close();
  
  void SetCloseOnDelete(bool value) { copying_input_.SetCloseOnDelete(value); }
  
  int GetErrno() { return copying_input_.GetErrno(); }
  
  // From InputStream
  bool Next(const void** data, int* size);
  void BackUp(int count);
  bool Skip(int count);
  int64_t ByteCount() const;
  
 private:
  class CopyFileInputStream : public CopyInputStream {
   public:
    CopyFileInputStream(int file_descriptor);
    ~CopyFileInputStream(); 
    
    bool Close();
    void SetCloseOnDelete(bool value) { close_on_delete_ = value; }
    int GetErrno() { return errno_; } 
    
    // From CopyInputStream
    int Read(void* buffer, int size);
    int Skip(int count);
    
   private:
    const int file_;
    bool close_on_delete_;
    bool is_closed_;
    
    int errno_;
    
    bool previous_seek_failed_;
    
    DISALLOW_COPY_AND_ASSIGN(CopyFileInputStream);
  };
  
  CopyFileInputStream copying_input_;
  CopyInputStreamAdaptor impl_;
  
  DISALLOW_COPY_AND_ASSIGN(FileInputStream);
};

} // namespace io
#endif // CRYPTO_IO_FILE_INPUT_STREAM_H_
