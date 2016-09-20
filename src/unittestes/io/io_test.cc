#include "unittestes/io/io_test.h"
#include "base/macros.h"

namespace io {


const int IoTest::kBlockSizes[] = {-1, 1, 2, 5, 7, 10, 23, 64};
const int IoTest::kBlockSizeCount = arraysize(IoTest::kBlockSizes);

bool IoTest::WriteToOutput(OutputStream* output,
                           const void* data, int size) {
  const uint8_t* in = reinterpret_cast<const uint8_t*>(data);
  int in_size = size;

  void* out;
  int out_size;

  while (true) {
    if (!output->Next(&out, &out_size)) {
      return false;
    }
    EXPECT_GT(out_size, 0);

    if (in_size <= out_size) {
      memcpy(out, in, in_size);
      output->BackUp(out_size - in_size);
      return true;
    }

    memcpy(out, in, out_size);
    in += out_size;
    in_size -= out_size;
  }
  return true;
}

#define MAX_REPEATED_ZEROS 100
  
int IoTest::ReadFromInput(InputStream* input, void* data, int size) {
  uint8_t* out = reinterpret_cast<uint8_t*>(data);
  int out_size = size;
  
  const void* in;
  int in_size = 0;
  
  int repeated_zeros = 0;
  
  while (true) {
    if (!input->Next(&in, &in_size)) {
      return size - out_size;
    }
    EXPECT_GT(in_size, -1);
    if (in_size == 0) {
      repeated_zeros++;
    } else {
      repeated_zeros = 0;
    }
    EXPECT_LT(repeated_zeros, MAX_REPEATED_ZEROS);
  
    if (out_size <= in_size) {
      memcpy(out, in, out_size);
      if (in_size > out_size) {
        input->BackUp(in_size - out_size);
      }
      return size;  // Copied all of it.
    }
  
    memcpy(out, in, in_size);
    out += in_size;
    out_size -= in_size;
  }
}
  
void IoTest::WriteString(OutputStream* output, const std::string& str) {
  EXPECT_TRUE(WriteToOutput(output, str.c_str(), str.size()));
}

  void IoTest::ReadString(InputStream* input, const std::string& str) {
    std::unique_ptr<char[]> buffer(new char[str.size() + 1]);
    buffer[str.size()] = '\0';
    EXPECT_EQ(ReadFromInput(input, buffer.get(), str.size()), str.size());
    EXPECT_STREQ(str.c_str(), buffer.get());
  }
  
  int IoTest::WriteStuff(OutputStream* output) {
    WriteString(output, "Hello world!\n");
    WriteString(output, "Some te");
    WriteString(output, "xt.  Blah blah.");
    WriteString(output, "abcdefg");
    WriteString(output, "01234567890123456789");
    WriteString(output, "foobar");
  
    EXPECT_EQ(output->ByteCount(), 68);
  
    int result = output->ByteCount();
    return result;
  }
  
  void IoTest::ReadStuff(InputStream* input) {
    ReadString(input, "Hello world!\n");
    ReadString(input, "Some text.  ");
    ReadString(input, "Blah ");
    ReadString(input, "blah.");
    ReadString(input, "abcdefg");
    EXPECT_TRUE(input->Skip(20));
    ReadString(input, "foo");
    ReadString(input, "bar");
    
    EXPECT_EQ(input->ByteCount(), 68);
    
    uint8_t byte;
    EXPECT_EQ(ReadFromInput(input, &byte, 1), 0);
  } 
  
  int IoTest::WriteStuffLarge(OutputStream* output) {
    WriteString(output, "Hello world!\n");
    WriteString(output, "Some te");
    WriteString(output, "xt.  Blah blah.");
    WriteString(output, std::string(100000, 'x'));  // A very long string
    WriteString(output, std::string(100000, 'y'));  // A very long string
    WriteString(output, "01234567890123456789");
  
    EXPECT_EQ(output->ByteCount(), 200055);
  
    int result = output->ByteCount();
    return result;
  }

void IoTest::ReadStuffLarge(InputStream* input) {
    ReadString(input, "Hello world!\nSome text.  ");
    EXPECT_TRUE(input->Skip(5));
    ReadString(input, "blah.");
    EXPECT_TRUE(input->Skip(100000 - 10));
    ReadString(input, std::string(10, 'x') + std::string(100000 - 20000, 'y'));
    EXPECT_TRUE(input->Skip(20000 - 10));
    ReadString(input, "yyyyyyyyyy01234567890123456789");
  
    EXPECT_EQ(input->ByteCount(), 200055);
  
    uint8_t byte;
    EXPECT_EQ(ReadFromInput(input, &byte, 1), 0);
}

} // namespace io
