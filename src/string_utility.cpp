#include "string_utility.hpp"

void copy_string(char *dst, const string_view &src) {
  memcpy(dst, src.data(), src.size());
  dst[src.size()] = 0;
}

int datify(const string_view str) {
  int ret = -1;
  switch (str[1]) {
    case '9' : {
      ret += 31;
      // fallthrough
    }
    case '8' : {
      ret += 31;
      // fallthrough
    }
    case '7' : {
      ret += 31;
      // fallthrough
    }
    case '6' : {
      break;
    }
    default : {
      return -1;
      break;
    }
  }
  return ret + (str[3] - '0') * 10 + str[4] - '0';
}

Time::Time() = default;
Time::Time(const string_view& x) : date(0), 
    time((x[0] - '0') * 240 + (x[1] - '0') * 24 + (x[3] - '0') * 10 + x[4] - '0') {}
Time::Time(std::nullptr_t) : date(-1), time(-1) {}
Time& Time::operator += (int x) {
  if (time == -1) return *this;
  time += x;
  date += time / (24 * 60);
  time %= (24 * 60);
}