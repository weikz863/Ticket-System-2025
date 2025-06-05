#pragma once

#ifndef _STRING_UTILITY_HPP_
#define _STRING_UTILITY_HPP_

#include <string>
#include <cstring>
#include <string_view>
using std::string, std::string_view;

void copy_string(char *dst, const string_view &src);



#endif