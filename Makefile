CXXFLAGS += -std=c++11
CXXFLAGS += -I./
CXXFLAGS += -I./src/
CXXFLAGS += -I./third_party/boringssl/include/
CXXFLAGS += -std=c++11 -Wall -Werror -g -c -o

LIB_FILES :=-lglog -lgflags -levent  -lpthread -lz -ldl \
	./third_party/boringssl/build/crypto/libcrypto.a \
	./third_party/boringssl/build/ssl/libssl.a \
	./third_party/boringssl/build/decrepit/libdecrepit.a \

TEST_LIB_FILES :=  -L/usr/local/lib -lgtest -lgtest_main -lpthread

PROTOC = protoc
GRPC_CPP_PLUGIN=grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTOS_PATH = ./protos

UNITTEST=./src/unittestes/

CPP_SOURCES := \
	./src/base/status.cc \
	./src/base/location.cc \
	./src/strings/string_piece.cc \
	./src/strings/string_encode.cc \
	./src/strings/stringprintf.cc \
	./src/strings/string_util.cc \
	./src/strings/strcat.cc \
	./src/strings/numbers.cc \
	./src/strings/scanner.cc \
	\
	./src/files/path.cc \
	./src/files/file_system.cc \
	./src/files/linux/linux_file_system.cc \
	\
	./src/system/load_library.cc \
	./src/system/threadpool.cc \
	./src/system/env.cc \
	./src/system/linux/linux_env.cc \
	\
	./src/io/input_stream.cc \
	./src/io/output_stream.cc \
	./src/io/array_input_stream.cc \
	./src/io/array_output_stream.cc \
	./src/io/string_input_stream.cc \
	./src/io/string_output_stream.cc \
	./src/io/copy_input_stream.cc \
	./src/io/copy_output_stream.cc \
	./src/io/file_input_stream.cc \
	./src/io/file_output_stream.cc \
	./src/unittestes/io/io_test.cc \
	./src/io/io_util.cc \
	\
	./src/crypto/openssl_util.cc \
	./src/crypto/ssl_aes_util.cc \
	./src/crypto/aes_encryptor.cc \
	./src/crypto/aes_key.cc \


CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)


TESTS := \
	$(UNITTEST)/crypto/aes_key_unittest \
	./src/unittestes/io/array_io_unittest \
	./src/unittestes/crypto/ssl_aes_util_unittest \

all: $(CPP_OBJECTS) $(TESTS)
.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

## Crypto
$(UNITTEST)/crypto/aes_key_unittest: \
	$(UNITTEST)/crypto/aes_key_unittest.o \
	./src/crypto/aes_key.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./src/unittests/crypto/aes_key_unittest.o: \
	./src/unittestes/crypto/aes_key_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./src/unittestes/crypto/ssl_aes_util_unittest: \
	./src/unittestes/crypto/ssl_aes_util_unittest.o \
	./src/crypto/ssl_aes_util.o \
	./src/crypto/aes_key.o \
	./src/io/io_util.o \
	./src/crypto/ssl_aes_util.h
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./src/unittestes/crypto/ssl_aes_util_unittest.o: \
	./src/unittestes/crypto/ssl_aes_util_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<


## IO
./src/unittestes/io/array_io_unittest: \
	./src/unittestes/io/array_io_unittest.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./src/unittestes/io/array_io_unittest.o: \
	./src/unittestes/io/array_io_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<


## /////////////////////////////

clean:
	rm -fr ./unittests/base/*.o
	rm -fr ./unittests/strings/*.o
	@rm -fr $(TESTS)
	@echo "rm *_unittest"
	@rm -fr $(CPP_OBJECTS)
	@echo "rm *.o"
