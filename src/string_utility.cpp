#include "string_utility.hpp"
#include <iostream>

void copy_string(char *dst, const string_view &src) {
  memcpy(dst, src.data(), src.size());
  dst[src.size()] = 0;
}

int datify(const string_view str) {
  int ret = 0;
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
      ret += 30;
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

inline string to_string_2(int x) {
  return {static_cast<char>('0' + x / 10), static_cast<char>('0' + x % 10)};
}

inline string to_date(int x) {
  if (x <= 0) return "xx-xx";
  if (x <= 30) return "06-" + to_string_2(x);
  if (x <= 61) return "07-" + to_string_2(x - 30);
  if (x <= 92) return "08-" + to_string_2(x - 61);
  else return "09-" + to_string_2(x - 92);
}

inline string to_time(int x) {
  if (x < 0) return "xx:xx";
  else return to_string_2(x / 60) + ':' + to_string_2(x % 60);
}

Time::Time() = default;
Time::Time(const string_view& x) : date(0), 
    time((x[0] - '0') * 600 + (x[1] - '0') * 60 + (x[3] - '0') * 10 + x[4] - '0') {}
Time::Time(std::nullptr_t) : date(-1), time(-1) {}
Time& Time::operator += (int x) {
  if (time == -1) return *this;
  time += x;
  date += time / (24 * 60);
  time %= (24 * 60);
  return *this;
}
int Time::operator - (const Time &other) const {
  return (date - other.date) * 24 * 60 + (time - other.time);
}

std::ostream& operator << (std::ostream &os, Time x) {
  return os << to_date(x.date) << ' ' << to_time(x.time);
}