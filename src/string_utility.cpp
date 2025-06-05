#include "string_utility.hpp"

void copy_string(char *dst, const string_view &src) {
  memcpy(dst, src.data(), src.size());
  dst[src.size()] = 0;
}