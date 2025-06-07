#pragma once

#ifndef _STRING_UTILITY_HPP_
#define _STRING_UTILITY_HPP_

#include <string>
#include <cstring>
#include <string_view>
using std::string, std::string_view;

void copy_string(char *dst, const string_view &src);

int datify(const string_view);

struct Time {
  int date, time;
  Time();
  Time(const string_view&);
  Time(std::nullptr_t);
  Time& operator += (int);
};

#endif