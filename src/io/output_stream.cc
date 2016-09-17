#include "io/output_stream.h"

namespace io {

OutputStream::~OutputStream() {}

bool OutputStream::WriteAliasedRaw(const void* /* data */,
		                   int /* size */) {
  return false;
}

} // namespace io
