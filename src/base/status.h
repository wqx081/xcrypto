#ifndef CORE_BASE_STATUS_H_
#define CORE_BASE_STATUS_H_
#include <iosfwd>
#include <string>


#include "strings/string_piece.h"


namespace base {

namespace error {
// These values must match error codes defined in google/rpc/code.proto.
enum Code {
  OK = 0,
  CANCELLED = 1,
  UNKNOWN = 2,
  INVALID_ARGUMENT = 3,
  DEADLINE_EXCEEDED = 4,
  NOT_FOUND = 5,
  ALREADY_EXISTS = 6,
  PERMISSION_DENIED = 7,
  UNAUTHENTICATED = 16,
  RESOURCE_EXHAUSTED = 8,
  FAILED_PRECONDITION = 9,
  ABORTED = 10,
  OUT_OF_RANGE = 11,
  UNIMPLEMENTED = 12,
  INTERNAL = 13,
  UNAVAILABLE = 14,
  DATA_LOSS = 15,
};
}  // namespace error

class Status {
 public:
  // Creates a "successful" status.
  Status();

  // Create a status in the canonical error space with the specified
  // code, and error message.  If "code == 0", error_message is
  // ignored and a Status object identical to Status::OK is
  // constructed.
  Status(error::Code error_code, strings::StringPiece error_message);
  Status(const Status&);
  Status& operator=(const Status& x);
  ~Status() {}

  // Some pre-defined Status objects
  static const Status OK;             // Identical to 0-arg constructor
  static const Status CANCELLED;
  static const Status UNKNOWN;

  // Accessor
  bool ok() const {
    return error_code_ == error::OK;
  }
  int error_code() const {
    return error_code_;
  }
  strings::StringPiece error_message() const {
    return error_message_;
  }

  bool operator==(const Status& x) const;
  bool operator!=(const Status& x) const {
    return !operator==(x);
  }

  void Update(const Status& new_status);

  // Return a combination of the error code name and message.
  std::string ToString() const;

 private:
  error::Code error_code_;
  std::string error_message_;
};

// Prints a human-readable representation of 'x' to 'os'.
std::ostream& operator<<(std::ostream& os, const Status& x);

#define EXPECT_OK(value) EXPECT_TRUE((value).ok())
#define RETURN_IF_ERROR(expr)                    \
	do {                                     \
          const base::Status _status = (expr);   \
          if (!_status.ok()) return _status;     \
        } while (0)

}  // namespace base
#endif  // CORE_BASE_STATUS_H_ 
