#ifndef CRYPTO_IO_FILE_OUTPUT_STREAM_H_
#define CRYPTO_IO_FILE_OUTPUT_STREAM_H_
#include "base/macros.h"
#include "io/output_stream.h"
#include "io/copy_output_stream.h"

namespace io {

class FileOutputStream : public OutputStream {
 public:
  explicit FileOutputStream(int file_descriptor, int block_size = -1);
  ~FileOutputStream();
  
  bool Close();
  
  bool Flush();
  
  void SetCloseOnDelete(bool value) { copying_output_.SetCloseOnDelete(value); }
  
  int GetErrno() { return copying_output_.GetErrno(); }
  
  bool Next(void** data, int* size);
  void BackUp(int count);
  int64_t ByteCount() const;
  
 private:
  class CopyFileOutputStream : public CopyOutputStream {
   public:
    CopyFileOutputStream(int file_descriptor);
    ~CopyFileOutputStream(); 
    
    bool Close();
    void SetCloseOnDelete(bool value) { close_on_delete_ = value; }
    int GetErrno() { return errno_; } 
    
    bool Write(const void* buffer, int size);
    
   private:
    const int file_;
    bool close_on_delete_;
    bool is_closed_;
    
    int errno_;
    
    DISALLOW_COPY_AND_ASSIGN(CopyFileOutputStream);
  };
  
  CopyFileOutputStream copying_output_;
  CopyOutputStreamAdaptor impl_;
  
  DISALLOW_COPY_AND_ASSIGN(FileOutputStream);
};

} // namespace io
#endif // CRYPTO_IO_FILE_OUTPUT_STREAM_H_
