#ifndef CRYPTO_IO_STRING_OUTPUT_STREAM_H_
#define CRYPTO_IO_STRING_OUTPUT_STREAM_H_
#include "base/macros.h"
#include "base/callback.h"

#include "io/output_stream.h"

#include <string>

namespace io {

class StringOutputStream : public OutputStream {
 public:
  explicit StringOutputStream(std::string* target);
  ~StringOutputStream();

  // From OutputStream
  bool Next(void** data, int* size);
  void BackUp(int count);
  int64_t ByteCount() const;

 protected:
  void SetString(std::string* target);

 private:
  DISALLOW_COPY_AND_ASSIGN(StringOutputStream);
  static const int kMinimumSize = 16;
  std::string* target_;
};

class LazyStringOutputStream : public StringOutputStream {
 public:
  explicit LazyStringOutputStream(base::ResultCallback<std::string*>* callback);
  ~LazyStringOutputStream();

  bool Next(void** data, int* size);
  int64_t ByteCount() const;

 private:
  const std::unique_ptr<base::ResultCallback<std::string*> > callback_;
  bool string_is_set_;

  DISALLOW_COPY_AND_ASSIGN(LazyStringOutputStream);
};

inline char* mutable_string_data(std::string* s) {
  return &(*s)[0];
}

} // namespace io
#endif // CRYPTO_IO_STRING_OUTPUT_STREAM_H_
