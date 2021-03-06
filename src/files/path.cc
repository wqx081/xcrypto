#include "files/path.h"
#include "strings/strcat.h"

namespace files {
namespace internal {

string JoinPathImpl(std::initializer_list<StringPiece> paths) {
  string result;

  for (StringPiece path : paths) {
    if (path.empty()) continue;

    if (result.empty()) {
      result = path.ToString();
      continue;
    }

    if (result[result.size() - 1] == '/') {
      if (IsAbsolutePath(path)) {
        strings::StrAppend(&result, path.substr(1));
      } else {
        strings::StrAppend(&result, path);
      }
    } else {
      if (IsAbsolutePath(path)) {
        strings::StrAppend(&result, path);
      } else {
        strings::StrAppend(&result, "/", path);
      }
    }
  }

  return result;
}

// Return the parts of the path, split on the final "/".  If there is no
// "/" in the path, the first part of the output is empty and the second
// is the input. If the only "/" in the path is the first character, it is
// the first part of the output.
std::pair<StringPiece, StringPiece> SplitPath(StringPiece path) {
  auto pos = path.rfind('/');

  // Handle the case with no '/' in 'path'.
  if (pos == StringPiece::npos)
    return std::make_pair(StringPiece(path.data(), 0), path);

  // Handle the case with a single leading '/' in 'path'.
  if (pos == 0)
    return std::make_pair(StringPiece(path.data(), 1),
                          StringPiece(path.data() + 1, path.size() - 1));

  return std::make_pair(
      StringPiece(path.data(), pos),
      StringPiece(path.data() + pos + 1, path.size() - (pos + 1)));
}

// Return the parts of the basename of path, split on the final ".".
// If there is no "." in the basename or "." is the final character in the
// basename, the second value will be empty.
std::pair<StringPiece, StringPiece> SplitBasename(StringPiece path) {
  path = Basename(path);

  auto pos = path.rfind('.');
  if (pos == StringPiece::npos)
    return std::make_pair(path, StringPiece(path.data() + path.size(), 0));
  return std::make_pair(
      StringPiece(path.data(), pos),
      StringPiece(path.data() + pos + 1, path.size() - (pos + 1)));
}
}  // namespace internal

bool IsAbsolutePath(StringPiece path) {
  return !path.empty() && path[0] == '/';
}

StringPiece Dirname(StringPiece path) {
  return internal::SplitPath(path).first;
}

StringPiece Basename(StringPiece path) {
  return internal::SplitPath(path).second;
}

StringPiece Extension(StringPiece path) {
  return internal::SplitBasename(path).second;
}

string CleanPath(StringPiece unclean_path) {
  string path = unclean_path.ToString();
  const char *src = path.c_str();
  string::iterator dst = path.begin();

  // Check for absolute path and determine initial backtrack limit.
  const bool is_absolute_path = *src == '/';
  if (is_absolute_path) {
    *dst++ = *src++;
    while (*src == '/') ++src;
  }
  string::const_iterator backtrack_limit = dst;

  // Process all parts
  while (*src) {
    bool parsed = false;

    if (src[0] == '.') {
      //  1dot ".<whateverisnext>", check for END or SEP.
      if (src[1] == '/' || !src[1]) {
        if (*++src) {
          ++src;
        }
        parsed = true;
      } else if (src[1] == '.' && (src[2] == '/' || !src[2])) {
        // 2dot END or SEP (".." | "../<whateverisnext>").
        src += 2;
        if (dst != backtrack_limit) {
          // We can backtrack the previous part
          for (--dst; dst != backtrack_limit && dst[-1] != '/'; --dst) {
            // Empty.
          }
        } else if (!is_absolute_path) {
          // Failed to backtrack and we can't skip it either. Rewind and copy.
          src -= 2;
          *dst++ = *src++;
          *dst++ = *src++;
          if (*src) {
            *dst++ = *src;
          }
          // We can never backtrack over a copied "../" part so set new limit.
          backtrack_limit = dst;
        }
        if (*src) {
          ++src;
        }
        parsed = true;
      }
    }

    // If not parsed, copy entire part until the next SEP or EOS.
    if (!parsed) {
      while (*src && *src != '/') {
        *dst++ = *src++;
      }
      if (*src) {
        *dst++ = *src++;
      }
    }

    // Skip consecutive SEP occurrences
    while (*src == '/') {
      ++src;
    }
  }

  // Calculate and check the length of the cleaned path.
  int path_length = dst - path.begin();
  if (path_length != 0) {
    // Remove trailing '/' except if it is root path ("/" ==> path_length := 1)
    if (path_length > 1 && path[path_length - 1] == '/') {
      --path_length;
    }
    path.resize(path_length);
  } else {
    // The cleaned path is empty; assign "." as per the spec.
    path.assign(1, '.');
  }
  return path;
}

}  // namespace files
