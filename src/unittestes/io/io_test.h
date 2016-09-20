#include "io/input_stream.h"
#include "io/output_stream.h"

#include <glog/logging.h>
#include <gtest/gtest.h>

namespace io {

class IoTest : public testing::Test {
 protected:
  bool WriteToOutput(OutputStream* output, const void* data, int size);
  int ReadFromInput(InputStream* input, void* data, int size);

  void WriteString(OutputStream* output, const std::string& str);
  void ReadString(InputStream* input, const std::string& str);

  int WriteStuff(OutputStream* output);
  void ReadStuff(InputStream* input);

  int WriteStuffLarge(OutputStream* output);
  void ReadStuffLarge(InputStream* input);

  static const int kBlockSizes[];
  static const int kBlockSizeCount;
};


} // namespace io
