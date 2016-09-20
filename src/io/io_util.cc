#include "io/io_util.h"
#include <string.h>

#include <glog/logging.h>

namespace io {

int IOUtil::ReadFromInput(InputStream* input,
                          void* data,
                          int size) {
  uint8_t* out = reinterpret_cast<uint8_t*>(data);
  int out_size = size;

  const void* in;
  int in_size = 0;

  while (true) {
    if (!input->Next(&in, &in_size)) {
      return size - out_size;
    }

    if (out_size <= in_size) {
      memcpy(out, in, out_size);
      if (in_size > out_size) {
        input->BackUp(in_size - out_size);
      }
      return size;
    }

    memcpy(out, in, in_size);
    out += in_size;
    out_size -= in_size;
  }
}

bool IOUtil::PeekInput(InputStream* input) {
  const void* in;
  int in_size = 0;
  if (!input->Next(&in, &in_size)) {
    return false;
  }
  input->BackUp(in_size);
  return true;
}

bool IOUtil::WriteToOutput(OutputStream* output,
                           const void* data,
                           int size) {
  const uint8_t* in = reinterpret_cast<const uint8_t*>(data);
  int in_size = size;

  void* out;
  int out_size;

  while (true) {
    if (!output->Next(&out, &out_size)) {
      return false;
    } 

    if (in_size <= out_size) {
      memcpy(out, in, in_size);
      output->BackUp(out_size - in_size);
      return true;
    }

    memcpy(out, in, out_size);
    in += out_size;
    in_size -= out_size;
  }
}

} // namespace io
