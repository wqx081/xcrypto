#include <dlfcn.h>
#include "system/load_library.h"

namespace core {

base::Status LoadLibrary(const char* library_filename, void** handle) {
  *handle = dlopen(library_filename, RTLD_NOW | RTLD_LOCAL | RTLD_NOLOAD);
  if (*handle) {
    return base::Status(base::error::ALREADY_EXISTS, "has already been loaded");
  }
  *handle = dlopen(library_filename, RTLD_NOW | RTLD_LOCAL);
  if (*handle) {
    return base::Status(base::error::NOT_FOUND, "Not found " + std::string(library_filename));
  }
  return base::Status::OK;
}

base::Status GetSymbolFromLibrary(void* handle, const char* symbol_name,
		            void** symbol) {
  *symbol = dlsym(handle, symbol_name);
  if (!*symbol) {
    return base::Status(base::error::NOT_FOUND, "Not found " + std::string(dlerror()));
  }   
  return base::Status::OK;
}

string FormatLibraryFileName(const string& name, const string& version) {
  string filename;
  if (version.size() == 0) {
    filename = "lib" + name + ".so";
  } else {
    filename = "lib" + name + ".so" + "." + version;
  }   
  return filename;
}

} // namespace core
