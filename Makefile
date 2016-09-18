CXXFLAGS += -std=c++11
CXXFLAGS += -I./src/
CXXFLAGS += -std=c++11 -Wall -Werror -g -c -o

LIB_FILES :=-lglog -lgflags -levent  -lpthread -lssl -lcrypto -lz
TEST_LIB_FILES :=  -L/usr/local/lib -lgtest -lgtest_main -lpthread

PROTOC = protoc
GRPC_CPP_PLUGIN=grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`
PROTOS_PATH = ./protos

CPP_SOURCES := \
	./src/base/status.cc \
	./src/strings/string_piece.cc \
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
	\
	./src/crypto/ssl_aes_key.cc \
	./src/crypto/aes_encryptor.cc \


CPP_OBJECTS := $(CPP_SOURCES:.cc=.o)


TESTS := \


all: $(CPP_OBJECTS) $(TESTS)
.cc.o:
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./unittests/strings/string_piece_unittest: \
	./unittests/strings/string_piece_unittest.o \
	./strings/string_piece.h \
	./strings/string_piece.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/strings/string_piece_unittest.o: \
	./unittests/strings/string_piece_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./unittests/strings/split_unittest: \
	./unittests/strings/split_unittest.o \
	./strings/split.h \
	./strings/split.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/strings/split_unittest.o: \
	./unittests/strings/split_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./unittests/event/event_loop_unittest: \
	./unittests/event/event_loop_unittest.o \
	./event/event_loop_libevent.h \
	./event/event_loop_interface.h \
	./event/event_loop_libevent.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/event/event_loop_unittest.o: \
	./unittests/event/event_loop_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./unittests/net/tcp_server_socket_unittest: \
	./unittests/net/tcp_server_socket_unittest.o \
	./event/event_loop_libevent.h \
	./event/event_loop_libevent.o \
	./net/test_completion_callback.o \
	./event/io_event_loop.h \
	./base/run_loop.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/net/tcp_server_socket_unittest.o: \
	./unittests/net/tcp_server_socket_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<


./unittests/server/http/http_server_unittest: \
	./unittests/server/http/http_server_unittest.o \
	./server/http/http_server.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/server/http/http_server_unittest.o: \
	./unittests/server/http/http_server_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<

./unittests/event/io_event_loop_unittest: \
	./unittests/event/io_event_loop_unittest.o \
	./event/io_event_loop.o \
	./event/event_loop_libevent.o \
	./event/io_event_loop.h \
	./base/run_loop.o \
	./base/run_loop.h
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/event/io_event_loop_unittest.o: \
	./unittests/event/io_event_loop_unittest.cc
	@echo "  [CXX]  $@"
	@$(CXX) $(CXXFLAGS) $@ $<


./unittests/threading/watchdog_unittest: \
	./unittests/threading/watchdog_unittest.o
	@echo "  [LINK] $@"
	@$(CXX) -o $@ $< $(CPP_OBJECTS) $(LIB_FILES) $(TEST_LIB_FILES)
./unittests/threading/watchdog_unittest.o: \
	./unittests/threading/watchdog_unittest.cc
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
