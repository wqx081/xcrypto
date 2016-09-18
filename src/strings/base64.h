#ifndef TENSORFLOW_LIB_STRINGS_B64_H_
#define TENSORFLOW_LIB_STRINGS_B64_H_

#include <string>
#include "base/string_piece.h"
#include "base/status.h"

namespace strings {

base::Status Base64Encode(base::StringPiece data, bool with_padding, 
		    std::string* encoded);
base::Status Base64Encode(base::StringPiece data, 
		    std::string* encoded);  // with_padding=false.

base::Status Base64Decode(base::StringPiece data, 
		    std::string* decoded);

}  // namespace mr

#endif  // TENSORFLOW_LIB_STRINGS_B64_H_
